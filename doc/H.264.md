# ffmpeg转换h264命令

```shell
ffmpeg -i NewRules.mp4 -vcodec copy -f h264 NewRules.264
```

# 裁剪h264流

```
ffmpeg -ss 0:0:0 -t 0:0:10 -i NewRules.264 -vcodec copy NewRules_cut.264
```

