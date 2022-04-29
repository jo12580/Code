# coding:utf-8
"""　　 api包:主要是实现http服务器，提供api接口(通过get请求,返回json数据)

　　 data文件夹:主要是数据库文件的存储位置和qqwry.dat(可以查询ip的地理位置)

　　 db包：主要是封装了一些数据库的操作

　　 spider包：主要是爬虫的核心功能，爬取代理网站上的代理ip

　　 test包：测试一些用例，不参与整个项目的运行

　　 util包：提供一些工具类。IPAddress.py查询ip的地理位置

　　 validator包:用来测试ip地址是否可用

　　 config.py：主要是配置信息(包括配置ip地址的解析方式和数据库的配置)"""

from multiprocessing import Value, Queue, Process
from api.apiServer import start_api_server
from db.DataStore import store_data

from validator.Validator import validator, getMyIP
from spider.ProxyCrawl import startProxyCrawl

from config import TASK_QUEUE_SIZE

if __name__ == "__main__":
    myip = getMyIP()
    DB_PROXY_NUM = Value('i', 0)
    q1 = Queue(maxsize=TASK_QUEUE_SIZE)
    q2 = Queue()
    p0 = Process(target=start_api_server)
    p1 = Process(target=startProxyCrawl, args=(q1, DB_PROXY_NUM,myip))
    p2 = Process(target=validator, args=(q1, q2, myip))
    p3 = Process(target=store_data, args=(q2, DB_PROXY_NUM))
    p0.start()
    p1.start()
    p2.start()
    p3.start()
    p0.join()
    p1.join()
    p2.join()
    p3.join()
