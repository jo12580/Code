#!/usr/bin/python3

#@Readme : 反爬之headers的伪装

# 对于检测Headers的反爬虫
from fake_useragent import UserAgent   # 下载：pip install fake-useragent

ua = UserAgent()        # 实例化，需要联网但是网站不太稳定-可能耗时会长一些

# 1.生成指定浏览器的请求头
print(ua.ie)
print(ua.opera)
print(ua.chrome)
print(ua.google)
print(ua.firefox)
print(ua.safari)
# 随机打印一个浏览器的User-Agent
print(ua.random)
print('完毕。')

# 2.在工作中常用的则是ua.random方式
import requests
ua = UserAgent()
print(ua.random)  # 随机产生

headers = {
    'User-Agent': ua.random    # 伪装
    }

# 请求
url = 'http://jo12580.icu.ltd/'
response = requests.get(url, headers=headers)
print(response.status_code)


