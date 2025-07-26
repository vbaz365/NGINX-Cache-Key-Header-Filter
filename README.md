# NGINX Cache Key Filter Module

This NGINX module adds a custom HTTP response header (`X-Cache-Key`) containing the internal cache key used by NGINX.

---

## Features

- Puts `X-Cache-Key` header into responses served from cache.

---

### 🔧 Installation

1. **Clone NGINX Source:**
```bash
git clone https://github.com/nginx/nginx.git
cd nginx
```
2. **Add the module**
```bash
./configure --add-module=path/to/the/module
```
3. **Make and make install**
```bash
sudo make
sudo make install
```
4. **Start NGINX**
```bash
sudo ./objs/nginx -c ./conf/nginx.conf
```

---

## Directive

- The directive is cache_key_filter
- Doesnt take any arguments
- Can be used in http, server or location blocks
- Example response is X-Cache-Key: 924b7ae76f9b901225b91c9c58d77c6f

