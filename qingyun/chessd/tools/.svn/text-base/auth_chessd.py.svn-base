#!/usr/bin/env python

import sys, os, string, struct, logging, md5, traceback, time

from pyPgSQL import PgSQL

server_host = 'xadrezlivre.c3sl.ufpr.br'

DB_SERVER = 'localhost'
DB_PORT = '5432'
DB_NAME = 'database'
DB_USER = 'user'
DB_PASS = 'password'

caminho = sys.argv[0].split('/')
caminho = caminho[:len(caminho)-1]
caminho.append('log_auth')
f = file('/'.join(caminho), 'aw')

""" Write log """
def log(msg, *args, **kwargs):
    f.write(str(msg) + ' ' + str(args or '') + ' ' + str(kwargs or '') + '\n')
    f.flush()

""" Log exception """
def log_error(exception):
    f.write(str(exception) + '\n')
    traceback.print_exc(file=f)
    f.flush()

""" Read the exactly nunbers of byte from file """
def read_exactly(f, bytes):
    r = ''
    while len(r) < bytes:
        tmp = f.read(bytes - len(r))
        if tmp == '':
            return ''
        r += tmp
    return r

def write_number(n):
    sys.stdout.write(struct.pack('>hh', 2, n))

"""
    Read input from ejabberd
    The input is a number two bytes long
    folowed by a string that number of bytes.
    The string is a sequence of values separated
    by ':' that represent a command
"""
def read_input():
    while 1:
        tam = read_exactly(sys.stdin, 2)
        if tam == '':
            return
        tam = struct.unpack('>h', tam)[0]
        cmd = read_exactly(sys.stdin, tam)
        if cmd == '':
            return
        cmd = cmd.split(':')
        yield tuple(cmd)


def calc_md5(str):
    md = md5.new()
    md.update(str)
    return md.digest().encode('hex')

""" Access to the user database """
class Users:
    def __init__(self):
        self.connect()

    def connect(self):
        feito = False
        while not feito:
            try:
                log('connecting')
                self.db = PgSQL.connect('%s:%s:%s:%s:%s' % (DB_SERVER,DB_PORT,DB_NAME,DB_USER,DB_PASS))
                self.st = self.db.cursor()
            except PgSQL.DatabaseError, msg:
                log_error(msg)
                time.sleep(10)
                log('reconnecting')
            else:
                feito = True
                log('connected')

    def getPassword(self, username):
        feito = False
        while not feito:
            try:
                self.st.execute('SELECT passwd FROM chessduser WHERE username=%s', username)
            except PgSQL.OperationalError, msg:
                log_error(msg)
                time.sleep(10)
                log('retrying')
                self.connect()
            else:
                feito = True
        passwd = self.st.fetchone()
        if passwd == None:
            return None;
        return passwd['passwd']
    
    def hasUser(self, username):
        passwd = self.getPassword(username)
        log(passwd)
        return passwd != None

    def registerUser(self, username, password):
        if self.hasUser(username):
            return False
        self.st.execute('SELECT standart_register(%s, NULL, %s, NULL)', username, calc_md5(password))
        self.db.commit()
        return True

"""
    Read each command and proccess it
    Two output is a number two bytes long,
    1 on success 0 on failure.

"""
def main():
    dbase = Users()
    for cmd in read_input():
        op = cmd[0]
        username = cmd[1]
        host = cmd[2]
        log('incoming', op=op, username=username, host=host)
        if host != server_host:
            log('Invalid server.')
            write_number(0)
        elif op == 'auth':
            """ Auth request """
            passwd = dbase.getPassword(username)
            given = cmd[3]
            if passwd != None and match_pass(passwd, given):
                log('ok')
                write_number(1)
            else:
                log('Wrong password.')
                write_number(0)
        elif op == 'isuser':
            """ Ask if the user exists """
            if dbase.hasUser(username):
                log('yes')
                write_number(1)
            else:
                log('no')
                write_number(0)
        elif op == 'register':
            password = cmd[3]
            if dbase.registerUser(username, password) == True:
                write_number(1)
                log('ok')
            else:
                write_number(0)
                log('no')
        else:
            """
                Any other command is not supported 
                I think the only one missing is to
                register a user
            """
            log('fail')
            write_number(0)
        sys.stdout.flush()

def match_pass(stored, given):
    if len(stored) == 32:
        return match_md5(stored, given)
    else:
        salt = stored[3:5]
        crypt = stored[6:]
        return match_chessd(salt, crypt, given)

def match_md5(stored, given):
    md = md5.new()
    md.update(given)
    return md.digest().encode('hex') == stored

def iter_div(n):
    while n>0:
        yield n
        n /= 2

def encode64(number, tam):
    table = './0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'
    r = ''
    for i in xrange(0, tam):
        r += table[number % 64]
        number /= 64
    return r


def match_chessd(salt, crypt, given):
    log('crypt ', {'original': crypt, 'gerada': crypt_chessd(salt, given)})
    return crypt_chessd(salt, given) == crypt

def crypt_chessd(salt, pw):
    magic = '$1$'
    MD5_SIZE = 16

    ctx1 = md5.new()
    ctx1.update(pw)
    ctx1.update(salt)
    ctx1.update(pw)
    final = ctx1.digest()


    ctx = md5.new()
    ctx.update(pw+magic+salt)

    for i in xrange(len(pw),0,-MD5_SIZE):
        ctx.update(final[:min(i, MD5_SIZE)])
    
    for i in iter_div(len(pw)):
        if i&1:
            ctx.update('\0')
        else:
            ctx.update(pw[0])

    final = ctx.digest()

    for i in xrange(0, 1000):
        ctx1 = md5.new()
        if i&1:
            ctx1.update(pw)
        else:
            ctx1.update(final)

        if i%3:
            ctx1.update(salt)

        if i%7:
            ctx1.update(pw)

        if i&1:
            ctx1.update(final)
        else:
            ctx1.update(pw)
        final = ctx1.digest()

    crypt = ''

    l = (ord(final[ 0])<<16) | (ord(final[ 6])<<8) | ord(final[12]);
    crypt += encode64(l,4);
    l = (ord(final[ 1])<<16) | (ord(final[ 7])<<8) | ord(final[13]);
    crypt += encode64(l,4);
    l = (ord(final[ 2])<<16) | (ord(final[ 8])<<8) | ord(final[14]);
    crypt += encode64(l,4);
    l = (ord(final[ 3])<<16) | (ord(final[ 9])<<8) | ord(final[15]);
    crypt += encode64(l,4);
    l = (ord(final[ 4])<<16) | (ord(final[10])<<8) | ord(final[ 5]);
    crypt += encode64(l,4);
    l =                    ord(final[11])                ;
    crypt += encode64(l,2);

    return crypt


if __name__ == '__main__':
    try:
        main()
    except Exception, error:
        log_error(error)
