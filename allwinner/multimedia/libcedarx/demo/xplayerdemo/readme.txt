asound.conf文件用于Linux方案，作为声音输出的配置文件，具体流程为：

1、alsa lib 会在pcm open声卡打开的函数里，对asound.conf文件进行加载，从而决定pcm音频流从何种音频设备输出。asound.conf文件配置了播放和录音的设备，客户可根据实际需要进行配置文件的修改，从而改变声音经何种设备输出，当前默认的播放设备为耳机输出：
pcm.PlaybackHP {
    type hooks
    slave.pcm "hw:audiocodec"
    hooks.0 {
	type ctl_elems
	hook_args [
	    {
	        name "HPOUT Switch"
	        preserve true
	        lock true
	        value 1
	    }
	]
    }
}
2、xplayerdemo在接收到pcm音频流时，tinasoundcontrol.c会调用snd_pcm_open(&sc->alsa_handler, "default",SND_PCM_STREAM_PLAYBACK ,mode)标准接口打开alsa声卡，传入“default”设备名称参数作为音频设备，跟asound.conf的default值相对应：
pcm.!default {
    type asym
    playback.pcm "PlaybackHP"
    capture.pcm "CaptureMIC"
}
若没有对应上，即使asound.conf被alsa库加载，也不会有声音输出，因此需要注意。

3、asound.conf集成后需要被打包在小机的/etc/目录下，alsa库会自动去加载。本地实验时，也可修改/etc/asound.conf进行音频设备配置的修改。