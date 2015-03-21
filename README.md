# Nginx配置

因为我的主目录是/home/liutos，而Eclipse的工作目录为~/workspace，所以这里的fastcgi_pass就写成这样子了

```nginx

        location ^~ /urlmark {
            include fastcgi_params;
            fastcgi_pass unix:/home/liutos/workspace/urlmark/urlmark.sock;
        }

```