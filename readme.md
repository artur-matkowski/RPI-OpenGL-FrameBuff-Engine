
**Failed to add service - already in use?**

As mentioned in [this issue](https://github.com/matusnovak/rpi-opengl-without-x/issues/1), you have to remove both `vc4-kms-v3d` and `vc4-fkms-v3d` from [R-Pi config](https://elinux.org/R-Pi_configuration_file). Also relevant discussion here: <https://stackoverflow.com/questions/40490113/eglfs-on-raspberry2-failed-to-add-service-already-in-use>. If you are using Raspberry Pi 4 (the `triangle_rpi4.c`) it might happen sometimes. I could not fully figure it out, but it might be due to incorrect EGL or DRM/GBM cleanup in  the code. I say "incorrect", but I don't know where. 



for usage of openGL ES 2.0 / opengl 2.1 vs DRI with error
"Failed to add service - already in use?"



or goto:
https://github.com/matusnovak/rpi-opengl-without-x



FOR EASY JUMP IN:
1. Run (or analize) configure.sh (it will install some dependencies)
2. You will need to have nvidia drivers installed (noaveu does not work for me)
3. Run `make build1-easy` it will build dependencies and the editor (player is not building currently)
4. Run `make run` to run the editor. You should be able to open a project (don't use last opened, they wont work for you  path ;) )
5. Select open project and navigate to `_exampleProject` directory. Hit OK
6. You should see tilted rectabgle.
7. ImGUI windows are kind of wonky at first, as they open is minimal areay, so you can berly notice them. In principle, you will have 3 windows: Hierarchy, Editor, Asapi Studio (+ ImGUI example window)
