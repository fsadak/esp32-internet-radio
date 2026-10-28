// Browser script smoke tests with deterministic DOM/network doubles; no hardware writes.
const fs=require('fs'),vm=require('vm'),assert=require('assert');
const page=fs.readFileSync('include/RadioWebPage.h','utf8').match(/<script>([\s\S]*?)<\/script>/)[1];
let checks=0;
async function test(kind){
 const elements=new Map();
 const element=()=>({textContent:'',value:'',files:[],disabled:false,inert:false,hidden:true,replaceChildren(){},append(){},setAttribute(){}});
 const get=id=>{if(!elements.has(id))elements.set(id,element());return elements.get(id);};
 const initial={version:'0.12.0',deviceId:'aabbccddeeff',token:'old',revision:'1',station:'Demo',track:'',source:'radio',stations:[],current:0,volume:5};
 let uploads=0,reloaded=false,confirmed=0;const headers={};
 const file=Buffer.alloc(512,1);file[0]=233;file[12]=9;file[13]=0;file[23]=1;file[32]=50;file[33]=84;file[34]=205;file[35]=171;
 if(kind==='badheader')file[0]=0;
 get('firmwareFile').files=[{name:'firmware.bin',size:kind==='oversize'?7000000:file.length,slice:(a,b)=>({arrayBuffer:async()=>new Uint8Array(file.subarray(a,b)).buffer})}];
 class XHR {
  constructor(){this.upload={};}open(method,path){assert.equal(method,'POST');assert.equal(path,'/api/firmware');}setRequestHeader(k,v){headers[k]=v;}
  send(body){uploads++;assert.equal(body,get('firmwareFile').files[0]);assert.equal(this.timeout,180000);this.upload.onprogress({lengthComputable:true,loaded:512,total:512});
   if(kind==='network'){this.onerror();return;}
   this.status=kind==='reject'?400:200;this.responseText=JSON.stringify({message:kind==='reject'?'Foreign firmware':'OK'});this.onload();
  }
 }
 const context=vm.createContext({console,document:{getElementById:get,querySelectorAll:()=>[],createElement:element},window:{addEventListener(){}},
  fetch:async path=>({ok:true,text:async()=>JSON.stringify(path==='/api/stations'?{csv:'name;url\n',revision:'1'}:path==='/api/firmware'?{maxSize:6553600}:initial),json:async()=>({...initial,token:'new'})}),
  confirm:()=>{confirmed++;return kind!=='cancel';},XMLHttpRequest:XHR,Uint8Array,TextDecoder,Blob,URL,
  location:{reload(){reloaded=true;}},AbortController,setTimeout:fn=>{queueMicrotask(fn);return 1;},clearTimeout(){},setInterval(){}});
 vm.runInContext(page,context);await vm.runInContext('queue',context);
 get('firmwareFile').onchange();
 await get('firmwareUpload').onclick();
 if(kind==='success'){
  assert.equal(uploads,1);assert.equal(reloaded,true);assert.equal(headers['X-Radio-Token'],'old');assert.equal(headers['X-Radio-Confirm'],'update');assert.equal(headers['Content-Type'],'application/octet-stream');assert.equal(get('firmwareProgress').value,100);
 }else{assert.equal(reloaded,false);assert.equal(uploads,['reject','network'].includes(kind)?1:0);if(['badheader','oversize'].includes(kind))assert.equal(confirmed,0);}
 assert.equal(get('firmwareFile').disabled,false);assert.equal(vm.runInContext('otaBusy',context),false);
 checks++;
}
(async()=>{for(const kind of ['success','badheader','oversize','cancel','reject','network'])await test(kind);console.log('PASS: '+checks+' browser OTA scenarios (success, invalid image, size, cancel, server rejection, connection failure)');})().catch(e=>{console.error(e);process.exit(1);});
