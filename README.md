# Supertext-transfer-protocol-sttp
As the name may not suggest, this is not http or anything. But a simple web server (or http server, idk what to call it) written in c. 


<p align="center">
<img  alt="demo-image" src="https://github.com/user-attachments/assets/f7de4cb1-5e59-4505-a3ae-a9d874ca713f" />
</p>


## features 
+ Configuration file (mappings)
  + Currently only redirect is supported
+ Static file viewer
+ Http error codes (some of them)
+ GET and HEAD response on fallback from mappings
+ Command Line Interface


## usage
```bash
sttp -r <path-to-site-root> -c <path-to-config>
# both root and config are required
# config can be left empty and you can point to any readable file (.txt, .yaml, .config) see example.conf to example
```

## build
```bash
git clone https://github.com/Tawhidulh1/sttp.git
cd sttp
make
# or install/uninstall to bin
make install  # Set custom path with PREFIX=... following make install/make uninstall
```
