#pragma once
#include <pgmspace.h>

// Self-contained page: no CDN, external fonts, or filesystem image upload.
static const char RADIO_WEB_PAGE[] PROGMEM = R"RADIOWEB(<!doctype html>
<html lang="tr"><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>İnternet Radyosu</title>
<style>
:root{color-scheme:dark;--orange:#ff7a00;--muted:#aaa}*{box-sizing:border-box}body{margin:0;background:#080808;color:#f5f5f5;font:16px system-ui,sans-serif}main{max-width:1050px;margin:auto;padding:24px}h1{font-size:25px;color:var(--orange)}h2{font-size:20px;margin-top:0}p{line-height:1.5}.muted,small{color:var(--muted)}section{background:#161616;border:1px solid #292929;border-radius:14px;padding:20px;margin:20px 0}button,input,select{font:inherit;border:1px solid #444;border-radius:9px;background:#222;color:inherit;padding:11px}button{cursor:pointer}button:hover{border-color:var(--orange)}button:disabled{opacity:.4;cursor:default}.accent{background:var(--orange);color:#080808;border-color:var(--orange)}.row{display:flex;gap:10px;align-items:center;flex-wrap:wrap}.spread{justify-content:space-between}#station{font-size:27px;margin:14px 0 4px}#track{color:var(--muted);min-height:24px}#volume{min-width:55px;text-align:center;font-size:26px}#playSelect{flex:1;min-width:190px}#message{padding:12px 0;min-height:46px;white-space:pre-wrap;color:var(--orange)}table{width:100%;border-collapse:collapse}td,th{padding:10px 5px;border-bottom:1px solid #303030;text-align:left}th{color:var(--muted);font-weight:normal}td input{width:100%}.actions{white-space:nowrap}.actions button{padding:8px}.scroll{overflow-x:auto}table{min-width:650px}.url{font-size:14px}.badge{color:var(--orange)}input[type=file]{max-width:100%}#save{margin-left:auto}#draft{color:var(--orange)}footer{color:var(--muted);font-size:13px;margin:22px 0}#track{overflow-wrap:anywhere}
@media(max-width:600px){main{padding:12px}section{padding:14px}h1{font-size:22px}#save{margin-left:0;width:100%}.actions button{padding:10px}}
</style>
<main><div class="row spread"><h1>♫ İnternet Radyosu</h1><span id="connection" class="muted">Bağlanıyor…</span></div>
<section><div class="row spread"><h2>Uzaktan kumanda</h2><span id="version" class="muted"></span></div>
<div id="station">—</div><div id="track"></div><p id="status" class="muted">Cihaz bekleniyor…</p>
<div class="row"><button data-command="previous">◀ Önceki</button><select id="playSelect" aria-label="Çalınacak istasyon"></select><button data-command="next">Sonraki ▶</button></div>
<p class="row"><button data-command="volumeDown" aria-label="Sesi azalt">−</button><strong id="volume">—</strong><button data-command="volumeUp" aria-label="Sesi artır">+</button><button id="mute" data-command="mute">Sessize al</button><button data-command="play">Yayını başlat</button><button data-command="stop">Durdur</button></p>
<small id="lock">Ekran ve web aynı radyo durumunu kullanır.</small></section>
<div id="message" role="status" aria-live="polite"></div>
<section id="editor"><div class="row spread"><h2>İstasyon yönetimi</h2><span id="draft">Kayıtlı liste</span></div>
<p class="muted">Değişiklikler önce bu sayfada hazırlanır. Cihaza kaydetmeden cihazdaki liste değişmez. En fazla 32 istasyon / 12 KB.</p>
<div class="row"><label>CSV yükle <input id="file" type="file" accept=".csv,text/csv"></label><button id="download">Kayıtlı CSV’yi indir</button></div>
<div class="scroll"><table><thead><tr><th>#</th><th>İstasyon adı</th><th>Doğrudan yayın adresi</th><th>İşlemler</th></tr></thead><tbody id="rows"></tbody></table></div>
<p class="row"><button id="add">+ İstasyon ekle</button><span id="count" class="muted"></span></p>
<div class="row"><button id="revert">Değişiklikleri geri al</button><button id="save" class="accent">Cihaza kaydet</button></div>
<p class="muted">CSV biçimi: UTF-8, noktalı virgül, başlık <code>name;url</code>. Dosya yükleme tüm taslağı değiştirir. İnternetteki yayın adresinin çalışması ayrıca denenmelidir.</p></section>
<footer>Bu sürümde uzaktan ses, sessize alma, istasyon seçimi ve yayın başlatma/durdurma vardır. Ekran, ekolayzer ve açılış tercihleri sonraki aşamada web üzerinden yönetilecek.</footer></main>
<script>
'use strict';
const $=id=>document.getElementById(id);
let token='',state=null,draft=[],baseRevision='',dirty=false,working=false,playerRevision='',queue=Promise.resolve();
function message(t){$('message').textContent=t;}
function changed(){dirty=true;$('draft').textContent='Kaydedilmemiş değişiklikler';}
function run(fn,freeze=false){const task=async()=>{working=true;if(freeze)$('editor').inert=true;try{await fn();}catch(e){message(e.message);}finally{if(freeze)$('editor').inert=false;working=false;}};queue=queue.then(task,task);return queue;}
async function api(path,body,headers={}){
 const response=await fetch(path,{method:body===undefined?'GET':'POST',cache:'no-store',headers:body===undefined?{}:{'Content-Type':'text/plain;charset=utf-8','X-Radio-Token':token,...headers},body});
 const raw=await response.text();let data;try{data=JSON.parse(raw);}catch{data={message:raw};}
 if(!response.ok){const e=new Error(data.message||'İstek tamamlanamadı.');e.code=data.code;throw e;}return data;
}
async function refresh(){
 state=await api('/api/state');token=state.token;
 $('connection').textContent='Cihaza bağlı';$('version').textContent='v'+state.version;
 $('station').textContent=state.station;$('track').textContent=state.track;
 $('status').textContent=[state.status,state.genre?'Tür: '+state.genre:'',state.format].filter(Boolean).join(' · ');
 $('volume').textContent=state.volume;$('mute').textContent=state.muted?'Sesi aç':'Sessize al';
 $('lock').textContent=state.source==='sd'?'SD müzik modunda kumandayı cihaz ekranından kullanın. Web değişiklikleri için Radyo kaynağına dönün.':state.editing?'Cihazda ayarlar açık. Web değişiklikleri için önce cihazdaki menüyü kapatın.':'Ekran ve web aynı radyo durumunu kullanır.';
 document.querySelectorAll('[data-command]').forEach(b=>b.disabled=state.editing);
 $('playSelect').disabled=state.editing;
 if(playerRevision!==state.revision){
  $('playSelect').replaceChildren();state.stations.forEach((name,i)=>{const o=document.createElement('option');o.value=i;o.textContent=(i+1)+'. '+name;$('playSelect').append(o);});playerRevision=state.revision;
 }
 $('playSelect').value=state.current;
 if(baseRevision&&state.revision!==baseRevision)$('draft').textContent='Cihazdaki liste değişti; yeniden yükleyin';
}
function parseCsv(csv){
 csv=csv.replace(/^\uFEFF/,'');let rows=[],row=[],field='',quoted=false;
 for(let i=0;i<csv.length;i++){
  const c=csv[i];
  if(c==='"'){if(quoted&&csv[i+1]==='"'){field+='"';i++;}else quoted=!quoted;}
  else if(c===';'&&!quoted){row.push(field);field='';}
  else if((c==='\r'||c==='\n')&&!quoted){row.push(field);if(row.some(v=>v.length))rows.push(row);row=[];field='';if(c==='\r'&&csv[i+1]==='\n')i++;}
  else field+=c;
 }
 if(field.length||row.length){row.push(field);rows.push(row);}
 return rows.slice(1).map(r=>({name:r[0],url:r[1]}));
}
function csv(){const quote=s=>'"'+s.replace(/"/g,'""')+'"';return 'name;url\n'+draft.map(s=>quote(s.name)+';'+quote(s.url)+'\n').join('');}
function render(){
 $('rows').replaceChildren();draft.forEach((s,i)=>{
  const tr=document.createElement('tr'),num=document.createElement('td');num.textContent=i+1;tr.append(num);
  for(const key of ['name','url']){const td=document.createElement('td'),input=document.createElement('input');input.value=s[key];input.className=key==='url'?'url':'';input.setAttribute('aria-label',(key==='name'?'İstasyon adı ':'Yayın adresi ')+(i+1));input.maxLength=key==='name'?96:512;input.oninput=()=>{s[key]=input.value;changed();};td.append(input);tr.append(td);}
  const actions=document.createElement('td');actions.className='actions';
  for(const [label,action] of [['↑',-1],['↓',1],['Sil',0]]){const b=document.createElement('button');b.textContent=label;b.disabled=(action===-1&&i===0)||(action===1&&i===draft.length-1);b.onclick=()=>{if(!action){if(!confirm('Bu istasyon taslaktan silinsin mi?'))return;draft.splice(i,1);}else [draft[i],draft[i+action]]=[draft[i+action],draft[i]];changed();render();};actions.append(b);}
  tr.append(actions);$('rows').append(tr);
 });$('count').textContent=draft.length+' / 32 istasyon';$('add').disabled=draft.length>=32;
}
async function load(){const data=await api('/api/stations');draft=parseCsv(data.csv);baseRevision=data.revision;dirty=false;$('draft').textContent='Kayıtlı liste';render();}
document.querySelectorAll('[data-command]').forEach(b=>b.onclick=()=>run(async()=>{const r=await api('/api/command',b.dataset.command);message(r.message);await refresh();}));
$('playSelect').onchange=()=>{const index=$('playSelect').value;run(async()=>{const r=await api('/api/command','select:'+index,{'X-Radio-Revision':playerRevision});message(r.message);await refresh();});};
$('add').onclick=()=>{if(draft.length>=32)return;draft.push({name:'',url:''});changed();render();};
$('revert').onclick=()=>run(async()=>{if(dirty&&!confirm('Kaydedilmemiş değişiklikler silinsin mi?'))return;await load();message('Cihazdaki kayıtlı liste alındı.');},true);
$('file').onchange=()=>run(async()=>{
 try{const file=$('file').files[0];if(!file)return;if(file.size>12288)throw new Error('Dosya 12 KB sınırını aşıyor.');if(dirty&&!confirm('Yüklenen dosya mevcut taslağın tamamını değiştirecek. Devam edilsin mi?'))return;
 const source=new TextDecoder('utf-8',{fatal:true}).decode(await file.arrayBuffer());const valid=await api('/api/validate',source);draft=parseCsv(valid.csv);changed();render();message('Dosya doğrulandı ve taslağa alındı. Henüz cihaza kaydedilmedi.');}finally{$('file').value='';}
},true);
$('download').onclick=()=>run(async()=>{const data=await api('/api/stations'),url=URL.createObjectURL(new Blob(['\ufeff'+data.csv],{type:'text/csv;charset=utf-8'}));const a=document.createElement('a');a.href=url;a.download='radyo-istasyonlari.csv';a.click();setTimeout(()=>URL.revokeObjectURL(url),1000);message('Cihazdaki kayıtlı liste indirildi.');});
$('save').onclick=()=>run(async()=>{
 const data=csv();let headers={'X-Radio-Revision':baseRevision},result;
 try{result=await api('/api/stations',data,headers);}catch(e){if(e.code!=='playing_removed')throw e;if(!confirm(e.message))return;result=await api('/api/stations',data,{...headers,'X-Radio-Confirm':'stop'});}
 await load();await refresh();message(result.message);
},true);
window.addEventListener('beforeunload',e=>{if(dirty){e.preventDefault();e.returnValue='';}});
run(async()=>{await refresh();await load();},true);
setInterval(()=>{if(!working)run(refresh);},2000);
</script></html>)RADIOWEB";
