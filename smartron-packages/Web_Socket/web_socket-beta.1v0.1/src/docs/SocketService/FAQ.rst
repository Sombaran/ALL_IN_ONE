FAQ
====================

1. What is discovery?
Ans. Discovery is process of on boarding Zigbee or Zwave device on IOT hub.

2. What is general process of discovery?
Ans. For discovery you need to start discovery from hub using Voice or Mobile app, and to put Smart Device in discovery mode you need to follow standard method came with smart device.

3. Can i Discover multiple device using discovery command one?
Ans. Of course you can, Web Socket use some internal mechanism to allow discover multiple device one by one, as long as you keep discovering smart device.


5. What is timeout for discovery?
Ans. Discovery timeout will be dynamic based on 20 Sec cycle, cycle will be reset based on weather any single  device discovered in that period of time.

6. What is control?
Ans. After Smart device discovered by IOT Hub, you can change your smart device properties like turning it ON or OFF or changing brightness or color.

7. How do I control devices?
Ans. there is some specific format of data that need to sent by Web Socket client so control the device, please find one such example below.

>>> import ws-client
>>> ws-client.control('Your Client ID', 'Your device control data')
>>> # Your device control data will in form of array, please refer format of command in developer API section

8. What is timeout for control a single device?
Ans. timeout for single device control is 3 Sec.


