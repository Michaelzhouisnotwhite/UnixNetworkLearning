

see the socket port status

```bash
# n: 以数字化显示 a:all t:tcp p:显示pid和进程名字
watch -n 1 "netstat -natp | grep 8089"
```