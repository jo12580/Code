# Wi-Fi Examples

This directory contains a range of examples ESP-IDF projects. These are intended to demonstrate the Wi-Fi functionality, and to provide code that you can copy and adapt into your own projects.

# Example Layout

The examples are grouped into sub-directories by category. Each category directory contains one or more example projects:

* `espnow` is the connectionless Wi-Fi communication protocol.——两个esp32连接
* `fast_scan` shows how to use fast scan while connecting to an AP.——wifi扫描
* `ftm` is a how to use FTM (Fine Timing Measurement) feature to calculate distance between devices.——WiFi测距
* `getting_started` contains the basic examples for using Wi-Fi in STA and AP mode.——基础功能，station(连接)与AP(热点)
* `iperf` shows the common performance measurements for two ESP running this example.——网络测试性能工具
* `power_save` contains a how to use power save mode of Wi-Fi.——节能管理
* `roaming` contains a example using 802.11k and 802.11v APIs.
* `scan` shows how to scan for all the available APs.
* `smart_config` shows how ESP connects to a target AP with ESPTOUCH.——指从已接入路由器的设备(通常是手机)，发送组播或广播。未接入路由器的Wi-Fi设备可以接收到这些信息，从而连接路由器，实现对设备的一键配网。
* `wifi_easy_connect` shows how to configure an ESP device as an enrollee using Device Provisioning Protocol (DPP).——扫码连接
* `wpa2_enterprise` connects to AP with WPA2 enterprise encryption.——加密连接
* `wps` contains a how to use the Wi-Fi Protected Setup protocol.——wps方式连接

# More

See the [README.md](../README.md) file in the upper level [examples](../) directory for more information about examples.
