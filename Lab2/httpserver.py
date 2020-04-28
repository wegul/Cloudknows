from socket import *
from concurrent.futures import ThreadPoolExecutor
import time
import os
import sys,getopt



def server(request):
    request = request.decode('utf-8')
    if len(request.split('\r\n', 1)) != 2:
        return
    request_line, body = request.split('\r\n', 1)
    request_head = body.split('\r\n\r\n', 1)[0] 
    request_body = body.split('\r\n\r\n', 1)[1]
    header_list = request_line.split(' ')
    method = header_list[0].upper()
    url = header_list[1]
    nowPath = os.getcwd()
    resHead = ''
    response_head = dict()
    response_head['Server'] = 'My server'
    
#    print(nowPath)
#    print(method)
#    print(url)
    
    if method == "GET":
        requestPath = nowPath + url
        if not os.path.isfile(requestPath):
            #print(1)
            f = open(nowPath + '/404.html', 'r')
            response_line = 'HTTP/1.1 404 Not Found\r\n'
            response_head['Content-Type'] = 'text/html'
            response_body = f.read()
            f.close()
        else:
            #print(2)
            f = open(requestPath, 'r')
            response_line = 'HTTP/1.1 200 OK\r\n'
            response_head['Content-Type'] = 'text/html'
            response_body = f.read()
            f.close()
           # print(response_body)
    elif method == "POST":
        try:
            name = request_body.split('&')[0]
            ID = request_body.split('&')[1]
            #print(name, ID)
            if url != '/Post_show' or name.split('=')[0].upper() != 'NAME' or ID.split('=')[0].upper() != 'ID':
                f = open(nowPath + '/404.html', 'r')
                response_line = 'HTTP/1.1 404 Not Found\r\n'
                response_head['Content-Type'] = 'text/html'
                response_body = f.read()
                f.close()
            else:
                response_line = 'HTTP/1.1 200 OK\r\n'
                response_head['Content-Type'] = 'text/html'
                response_body = '<html>\r\n<title>POST Method</title>\r\n<body bgcolor="ffffff">\r\nYour Name: %s\r\nID: %s\r\n<hr><em>HTTP Web server</em>\r\n</body>\r\n<html>' % (name.split('=')[1], ID.split('=')[1])
                #print(666)
        except BaseException:
            f = open(nowPath + '/404.html', 'r')
            response_line = 'HTTP/1.1 404 Not Found\r\n'
            response_head['Content-Type'] = 'text/html'
            response_body = f.read()
            f.close()
    else:
        #print(666)
        f = open(nowPath + '/501.html', 'r')
        response_line = 'HTTP/1.1 501 Not Implemented\r\n'
        response_head['Content-Type'] = 'text/html'
        response_body = f.read()
        f.close()
        #print(response_body)

    for key in response_head:
        resHead = resHead + key + ': ' + response_head[key] + '\r\n'
    #print(response_line + resHead + '\r\n' + response_body)
    return response_line + resHead + '\r\n' + response_body



def tcpLink(sock, addr):
    request = sock.recv(1024)
    #print('connect')
    sock.send(server(request).encode('utf-8'))
    sock.close()

def main(argv):
    HOST = "127.0.0.1"
    numvalue = 32
    SRC_PORT = 8897
    BufferSize = 1024
    opts,args = getopt.getopt(sys.argv[1:],'',['ip=','port=','proxy=','number-thread='])
    for opt_name,opt_value in opts:
        if opt_name in ('--number-thread'):
            numvalue = int(opt_value)
            #print("num:" , numvalue)
        if opt_name in ('--ip'):
            HOST  = opt_value
            #print("ip:",HOST)
        if opt_name in ('--port'):
            SRC_PORT = int(opt_value)
            #print("port:" , SRC_PORT)
        if opt_name in ('--proxy'):
            proxyvalue = opt_value
            #print("proxy: ",proxyvalue)
    pool = ThreadPoolExecutor(numvalue)
    tcpServer = socket(AF_INET, SOCK_STREAM)
    ADDR = (HOST, SRC_PORT)
    tcpServer.bind(ADDR)
    tcpServer.listen(1000)
   # print("waiting for connecting")
    while True:
        tcpAccept, addr = tcpServer.accept()
        fu = pool.submit(tcpLink, *(tcpAccept, addr))

if __name__ == '__main__':
    main(sys.argv[1:])
