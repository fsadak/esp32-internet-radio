from http.server import BaseHTTPRequestHandler, HTTPServer
from pathlib import Path
import json, threading

LOG=Path(__file__).parent/'mock-requests.jsonl'
state={}
def reset():
    state.clear();state.update(version='0.11.0',deviceId='aabbccddeeff',deviceName='ESP32 Radyo ddeeff',token='local-test-token',revision='100',source='radio',station='TRT FM',track='Deneme yayını · benzetim',status='Yayın hazır',volume=11,maxVolume=21,muted=False,playing=True,editing=False,current=0,stations=['TRT FM','Radyo 7','Süper FM'],mode='normal')
reset()
class Handler(BaseHTTPRequestHandler):
    def log_message(self,*args):pass
    def reply(self,code,data):
        raw=(json.dumps(data,ensure_ascii=False) if not isinstance(data,str) else data).encode()
        self.send_response(code);self.send_header('Content-Type','application/json');self.send_header('Content-Length',str(len(raw)));self.send_header('Connection','close');self.end_headers();self.wfile.write(raw)
    def do_GET(self):
        if self.path.startswith('/test/mode?value='):
            m=self.path.split('=',1)[1]
            if m=='reset':reset()
            elif m=='revision':state.update(revision='101',mode='normal')
            elif m=='editing':state.update(editing=True,source='radio',mode='normal')
            elif m=='sd':state.update(editing=True,source='sd',mode='normal')
            else:state['mode']=m
            return self.reply(200,{})
        if self.path!='/api/state':return self.reply(404,{})
        if state['mode']=='error':return self.reply(503,{'message':'Mock service unavailable'})
        if state['mode']=='badjson':return self.reply(200,'not json')
        self.reply(200,{k:v for k,v in state.items() if k!='mode'})
    def do_POST(self):
        body=self.rfile.read(int(self.headers.get('Content-Length',0))).decode()
        entry={'path':self.path,'command':body,'length':self.headers.get('Content-Length'),'transfer':self.headers.get('Transfer-Encoding'),'host':self.headers.get('Host'),'token_ok':self.headers.get('X-Radio-Token')==state['token'],'revision_ok':self.headers.get('X-Radio-Revision')==state['revision']}
        with LOG.open('a',encoding='utf-8') as f:f.write(json.dumps(entry)+'\n')
        if state['mode']=='rotate':state.update(token='rotated-token',mode='normal');return self.reply(403,{'message':'Token expired'})
        if self.path!='/api/command':return self.reply(404,{})
        if not entry['token_ok']:return self.reply(403,{'message':'Token missing'})
        if self.headers.get('Transfer-Encoding') or self.headers.get('Content-Length') is None:return self.reply(400,{'message':'Length required'})
        if body=='volumeUp':state['volume']=min(state['maxVolume'],state['volume']+1)
        elif body=='volumeDown':state['volume']=max(0,state['volume']-1)
        elif body=='mute':state['muted']=not state['muted']
        elif body=='next':state['current']=(state['current']+1)%3
        elif body=='previous':state['current']=(state['current']-1)%3
        elif body=='play':state['playing']=True
        elif body=='stop':state['playing']=False
        elif body.startswith('select:'):
            if not entry['revision_ok']:return self.reply(409,{'message':'List changed'})
            state['current']=int(body[7:])
        else:return self.reply(400,{'message':'Unknown command'})
        state['station']=state['stations'][state['current']]
        self.reply(200,{'message':'Komut uygulandı.'})
print('Mock radio on 127.0.0.1:80',flush=True)
HTTPServer(('127.0.0.1',80),Handler).serve_forever()
