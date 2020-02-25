
**Failed to add service - already in use?**

As mentioned in [this issue](https://github.com/matusnovak/rpi-opengl-without-x/issues/1), you have to remove both `vc4-kms-v3d` and `vc4-fkms-v3d` from [R-Pi config](https://elinux.org/R-Pi_configuration_file). Also relevant discussion here: <https://stackoverflow.com/questions/40490113/eglfs-on-raspberry2-failed-to-add-service-already-in-use>. If you are using Raspberry Pi 4 (the `triangle_rpi4.c`) it might happen sometimes. I could not fully figure it out, but it might be due to incorrect EGL or DRM/GBM cleanup in  the code. I say "incorrect", but I don't know where. 



for usage of openGL ES 2.0 / opengl 2.1 vs DRI with error
"Failed to add service - already in use?"



or goto:
https://github.com/matusnovak/rpi-opengl-without-x

