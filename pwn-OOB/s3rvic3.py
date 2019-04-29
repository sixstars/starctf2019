#! /usr/bin/env python33
# -*- coding: utf-8 -*-
import socket,os,sys,signal
from multiprocessing import Process,Pipe
import tempfile
import pwd
import subprocess
import random,string,select
from hashlib import sha256
import smtplib,ssl
from email.mime.application import MIMEApplication
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.utils import COMMASPACE, formatdate
from time import sleep
import traceback
import _thread

TIME = 30
PORT = 10000
MAIL = "bot1@sixstars.team"
PASS = "PASSWORD_EXAMPLE"
MAIL_SERVER = "smtp.zoho.com"
MAIL_SERVER_PORT = 465
EXEC_USER = "bot"
DISPLAY = ":0"


def kill(proc_pid):
    process = psutil.Process(proc_pid)
    for proc in process.children(recursive=True):
        proc.kill()
    process.kill()

def proof_of_work(fd):
    proof = ''.join([random.choice(string.ascii_letters+string.digits) for _ in range(20)])
    digest = sha256(proof.encode('utf-8')).hexdigest()
    fd.send(("sha256(XXXX+%s) == %s\n" % (proof[4:],digest)).encode('utf-8'))
    fd.send(b'Give me XXXX:')
    #fd.send(proof[:4].encode("utf-8"))
    x = fd.recv(1024)
    x = x.strip().decode("utf-8")
    if len(x) != 4 or sha256((x+proof[4:]).encode('utf-8')).hexdigest() != digest:
        fd.send(b"Wrong\n")
        return False
    fd.send(b'OK\n')
    return True

def start_chrome(url,filename):
    print("start chrome!")
    cmd = "byzanz-record -d %d -x 0 -y 0 -w 1920 -h 1080 --display=%s %s"%(TIME,DISPLAY,filename)
    env = os.environ
    env["DISPLAY"]=DISPLAY
    record = subprocess.Popen(cmd.split(" "),env = env)
    chrome = subprocess.Popen(["timeout",str(TIME),"/opt/Chrome/chrome","--no-sandbox",
        "--window-position=0,0",
        "--window-size=1920,1080",
        url],
            env = env, stderr = subprocess.DEVNULL,stdout = subprocess.DEVNULL)
    try:
        chrome.communicate(timeout=TIME + 2)
    except Exception as e:
        print("chrome timeout")
        chrome.kill()
    chrome.terminate()
    record.terminate()
    sleep(1)
    print("chrome done!")

def send_vedio(filename,email):
    print("send %s to %s"%(filename,email))
    msg = MIMEMultipart()
    msg['From'] = MAIL
    msg['To'] = email
    msg['Date'] = formatdate(localtime=True)
    msg['Subject'] = "*CTF Chrome Vedio"
    body = """
        Here is your vedio.
    """
    msg.attach(MIMEText(body))
    name = "vedio.gif"
    with open(filename,"rb") as f:
        part = MIMEApplication(
            f.read(),
            name
                )
        part['Content-Disposition'] = 'attachment; filename="%s"' % name
        msg.attach(part)

    context = ssl.create_default_context()
    server =  smtplib.SMTP_SSL(MAIL_SERVER,MAIL_SERVER_PORT,context = context)
    server.ehlo()
    server.login(MAIL, PASS)
    server.sendmail(MAIL, email, msg.as_string())
    server.close()
    print("send done!")
def handler_client_connection(c,client_fd):
    c.settimeout(2*TIME)
    if not proof_of_work(c):
        c.close()
        return
    try:
        c.send(
b"""
 #   #    ####    #####  ######
  # #    #    #     #    #
### ###  #          #    #####
  # #    #          #    #
 #   #   #    #     #    #
          ####      #    # 
""")
        c.send(b"Welcome to *CTF Chrome Pwn! You can give me an URL, Bot will open it using chrome(no sandbox), pwn it and flag in \"/\"\n")
        c.send(b"Your URL:(start with \"http://\") ")
        url = c.recv(1024).strip().decode()
        c.send(b"Your Email: ")
        email = c.recv(1024).strip().decode()
        if (not url.startswith("http://")) \
                or len(url) > 100 \
                or len(email) > 100:
            c.send(b"Invalid!\n")
            raise Exception("URL invalid")
        print("url:%s,email:%s"%(url,email))
        client_fd.send([url,email])
        c.send(b"We will send the vedio of chrome to your email after 2 minutes. Good Luck!\n");
    except Exception as e:
        print("Service error:",e)
    c.close()

def client_do(server_fd,client_fd):
    server_fd.close()
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(('0.0.0.0',PORT))
    s.listen(50)
    while True:
        c, addr = s.accept()
        print('Got connection from', addr)
        _thread.start_new_thread(handler_client_connection,(c,client_fd))

def random_str(N):
    return ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(N))

def change_user(user):
    pw_record = pwd.getpwnam(user)
    user_uid = pw_record.pw_uid
    user_gid = pw_record.pw_gid
    os.setgid(user_gid)
    os.setuid(user_uid)
    os.environ["HOME"] = "/home/%s"%user

def main():
    server_fd,client_fd = Pipe()
    client = Process(target = client_do,args = (server_fd,client_fd))
    client.start()
    client_fd.close()
    print("Service Start")
    while True:
        _,_,_ = select.select([server_fd.fileno()],[],[])
        url,email = server_fd.recv()
        filename = "/tmp/%s.gif"%random_str(10)
        pid = os.fork()
        if pid == 0:
            try:
                change_user(EXEC_USER)
                start_chrome(url,filename)
                send_vedio(filename,email)
            except Exception as e:
                traceback.print_exc(file=sys.stdout)
                print("Server Err:", e)
            os._exit(0)
        else:
            os.waitpid(pid,0)

if __name__ == "__main__":
    main()
