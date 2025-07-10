for i in `ls *_aarch64-openwrt-linux-gnu.so`; do
    mv $i ${i%_aarch64-openwrt-linux-gnu.so}.so
done
