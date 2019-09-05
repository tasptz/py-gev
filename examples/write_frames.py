import gev
from PIL import Image

def main():
    devs = gev.Camera.list_devices()
    print('Devices', devs)

    with gev.Camera(devs[0]) as cam:
        print(cam.properties)
        cam.start_grab()
        try:
            for _ in range(5):
                idx, img = cam.recv(wait=True)
                Image.fromarray(img).save(f'frame_{idx:06d}.jpg')
                print(idx)
        finally:
            cam.stop_grab()

if __name__ == '__main__':
    main()
