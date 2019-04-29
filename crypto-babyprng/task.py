import os,random,sys,string
from hashlib import sha256
import SocketServer
import signal

from flag import FLAG

class Task(SocketServer.BaseRequestHandler):
    TH = 0.9 # overwritten!!
    DELTA = 0.005
    SIZE = 100000
    def proof_of_work(self):
        random.seed(os.urandom(8))
        proof = ''.join([random.choice(string.ascii_letters+string.digits) for _ in xrange(20)])
        digest = sha256(proof).hexdigest()
        self.request.send("sha256(XXXX+%s) == %s\n" % (proof[4:],digest))
        self.request.send('Give me XXXX:')
        x = self.request.recv(10)
        x = x.strip()
        if len(x) != 4 or sha256(x+proof[4:]).hexdigest() != digest: 
            return False
        return True

    def recvhex(self, sz):
        try:
            r = sz
            res = ''
            while r>0:
                res += self.request.recv(r)
                if res.endswith('\n'):
                    r = 0
                else:
                    r = sz - len(res)
            res = res.strip()
            res = res.decode('hex')
        except:
            res = ''
        return res

    def dosend(self, msg):
        try:
            self.request.sendall(msg)
        except:
            pass

    def randbit(self):
        if random.random()>self.TH:
            return 1
        else:
            return 0

    def run(self, code):
        stack = []
        out = []
        cnt = 0
        random.seed(os.urandom(8))
        self.TH = 0.7 + random.random()*0.25
        for _ in xrange(self.SIZE):
            stack.append(self.randbit())
        try:
            pos = 0
            for _ in xrange(self.SIZE*10):
                c = code[pos]
                pos += 1
                if c=='\x00':
                    out.append(stack[-1])
                elif c=='\x01':
                    if stack[-1]==1:
                        pos += 1
                elif c=='\x02':
                    del stack[-1]
                elif c=='\x03':
                    stack[-1] = stack[-1]&stack[-2]
                elif c=='\x04':
                    stack[-1] = stack[-1]|stack[-2]
                elif c=='\x05':
                    stack[-1] = stack[-1]^stack[-2]
                #elif c=='\x06':
                #    stack[-1] = 1-stack[-1]
                #elif c=='\x06':
                #    stack.append(stack[-1])
                elif ord(c)>=0x10 and ord(c)<=0x30:
                    pos += ord(c)-0x10
                elif ord(c)>=0x30 and ord(c)<=0x50:
                    pos -= ord(c)-0x30
        except:
            pass
        return out

    def handle(self):
        #if not self.proof_of_work():
        #    return
        #signal.alarm(3)
        self.dosend("opcode(hex): ")
        code = self.recvhex(400)
        out = self.run(code)
        print len(out)
        if len(out) > int(self.SIZE*0.9):
            one = float(len(filter(lambda x:x==1,out)))/len(out)
            print one
            if abs(one-0.5)<self.DELTA:
                self.dosend("%s\n" % FLAG)
            else:
                self.dosend(">.<\n")
        else:
            self.dosend(">.<\n")
        self.request.close()


class ForkingServer(SocketServer.ForkingTCPServer, SocketServer.TCPServer):
    pass


if __name__ == "__main__":
    HOST, PORT = '0.0.0.0', 10002
    print HOST
    print PORT
    server = ForkingServer((HOST, PORT), Task)
    server.allow_reuse_address = True
    server.serve_forever()
