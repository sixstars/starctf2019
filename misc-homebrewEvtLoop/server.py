#!/usr/bin/python
# -*- encoding: utf-8 -*-
# written in python 2.7
__author__ = 'garzon'

import sys
import hashlib
import random

# private ------------------------------------------------------------
def flag():
    # flag of stage 1
    return '*ctf{JtWCBuYlVN75pb]y8zhJem9GAH1YsUqgMEvQn_P2wd0IDRTaHjZ3i6SQXrxKkL4[FfocO}'

def flag2():
    ret = ''
    # flag of stage 2
    # ret = open('flag', 'rb').read() # No more flag for you hackers in stage2!
    return ret

def switch_safe_mode_factory():
    ctx = {'io_pair': [None, None]}
    def __wrapper(): (ctx['io_pair'], (sys.stdin, sys.stderr)) = ([sys.stdin, sys.stderr], ctx['io_pair'])
    return __wrapper

def PoW():
    #return
    while True:
        a = (''.join([chr(random.randint(0, 0xff)) for _ in xrange(2)])).encode('hex')
        print 'hashlib.sha1(input).hexdigest() == "%s"' % a
        print '>',
        input = raw_input()
        if hashlib.sha1(input).hexdigest()[:4] == a:
            break
        print 'invalid PoW, please retry'

# protected ----------------------------------------------------------
def fib(a):
    if a <= 1: return 1
    return fib(a-1)+fib(a-2)

# public -------------------------------------------------------------
def load_flag_handler(args):
    global session
    session['log'] = flag2()
    return 'done'

def ping_handler(args):
    return 'pong'

def fib_handler(args):
    a = int(args[0])
    if a > 5 or a < 0: return 'out of range'
    return str(fib(a))

if __name__ == '__main__':
    session = {}
    session['log'] = flag()
    switch_safe_mode = switch_safe_mode_factory()
    switch_safe_mode_factory = None
    valid_event_chars = set('abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789[]')

    while True:
        PoW()
        print '$',
        event = raw_input() # get eventName and args from the RPC requests, like: funcName114514arg1114514args2114514arg3 ...
        switch_safe_mode()
        if event == 'exit': break

        for c in event:
            if c not in valid_event_chars:
                print "invalid request"
                exit(-1)

        event, args = event.split('114514')
        args = args.split('114514')

        try:
            handler = eval(event)
            print handler(args)
        #except Exception, e:
        #	print 'exception:', str(e)
        except:
            print 'exception'

