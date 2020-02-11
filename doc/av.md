# 播放视频

```mermaid
graph TD
data[音视频数据]-->decode_protocol[解协议]
decode_protocol-->encapsulate_data[封装格式数据]
encapsulate_data-->decode_encaspsulation[解封装]
decode_encaspsulation-->desperate_audio_and_video{分离音视频数据}
desperate_audio_and_video-->|视频数据|video_compression_data[视频压缩数据]
desperate_audio_and_video-->|音频数据|audio_compression_data[音频压缩数据]
video_compression_data-->decode_video[视频解码]
audio_compression_data-->decode_audio[音频解码]
decode_video-->video_original_data[视频原始数据]
decode_audio-->audio_original_data[音频原始数据]
video_original_data-->audio_video_synchronization[音视频同步]
audio_original_data-->audio_video_synchronization[音视频同步]
audio_video_synchronization-->output_device[输出设备]
```

- 解协议：将流媒体协议数据，解析为标准的相应的封装格式数据。常见的流媒体协议有HTTP、RTMP、MMS等。比如RTMP传输的数据，解协议操作后输出FLV数据。
- 解封装：将输入的封装格式数据，分离成为音频流压缩编码数据和视频流压缩编码数据。常用的封装格式如MP4、MKV、RMVB、TS、FLV、AVI等。比如FLV数据解封装后，输出H.264编码的视频码流和AAC编码的音频码流。
- 解码：将音视频压缩编码的数据，解码为非压缩的音视频原始数据。常见的音频压缩编码标准有AAC、MP3、AC-3等，视频压缩编码标准则包含H.264、MPEG2、VC-1等。这是整个系统中最重要也是最复杂的一个环节，解码后输出为未压缩的颜色数据，如YUV420P、RGB等。
- 视音频同步：根据解封装模块处理过程中得到的参数信息，同步解码出来的视频和音频数据，将音频数据发送至声卡，视频发送至显卡播放出来。

# 视频格式

- [YUV](YUV.md)