import requests
requests.packages.urllib3.disable_warnings()
# https://quietistic-electrod.000webhostapp.com/
#"https://quietistic-electrod.000webhostapp.com/post1.php" # 
url = "http://192.168.1.106:3000/upload_firmware"
file_path = r"E:\OTA\OTA_3\build\esp32.esp32.esp32\OTA_3.ino.bin"
version = "2.0"  
# filename = 'OTA_3.ino.bin'
with open(file_path, "rb") as firmware_file:
    firmware_content = firmware_file.read()
response = requests.post(url, files={'OTA_3.ino.bin': firmware_content}, headers={'X-Version': version, 'Content-Disposition':'attachment; filename=OTA_3.ino.bin'})

if response.status_code == 200:
    print("Firmware file uploaded successfully")
else:
    print(f"Failed to upload firmware file. Status code: {response.status_code}")
