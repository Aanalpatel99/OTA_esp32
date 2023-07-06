from http.server import BaseHTTPRequestHandler, HTTPServer
# import socket
import os
import json

PORT = 3000

server_address = ('192.168.1.106', PORT) #192.168.1.8
UPLOAD_DIRECTORY = '/upload_firmware'
FILE_PATH = os.path.join(UPLOAD_DIRECTORY, 'OTA_3.ino.bin')
firmware_update = 0
version = 1.0
updatedDevices = []
mac_ID = 0
class MyHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == "/upload_firmware":
        
            self.send_response(200)

            self.send_header('Content-type', 'application/octet-stream')
            self.send_header('Content-Length', str(os.path.getsize(FILE_PATH)))
#             self.send_header('Content-Length', str(os.path.getsize(filepath)))

            self.send_header('Content-Disposition', 'attachment; filename=OTA_3.ino.bin')
            self.end_headers()
            
            with open(FILE_PATH, "rb") as firmware_file:
                firmware_data = firmware_file.read()
                print("Firmware size:", len(firmware_data))
                self.wfile.write(firmware_data)
        
        else:
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            self.wfile.write(bytes(str(version), "utf8"))
            print("version = {}".format(version))


    def do_POST(self):
        global firmware_update, version, updatedDevices, yetToUpdate, mac_ID
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        content_type = self.headers['Content-Type']
        if content_type == 'application/json': # JSON file
            print("json file method")
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length).decode()
            try:
                json_data = json.loads(post_data)
                print(json_data)
                mac_ID = json_data['mac_id']
                mac_version = float(json_data['Version'])
                if str(mac_version) == str(version):
                    updatedDevices.append(mac_ID)
                    updatedDevices = list(set(updatedDevices))
                print("updatedList = ", updatedDevices)
            except json.JSONDecodeError as e:
                print("Error decoding JSON:", e)
        else:    
            content_length = int(self.headers['Content-Length'])
            if version != self.headers['X-Version']: 
                updatedDevices.clear()
            version = self.headers['X-Version']
            print(version)
            filename = None
            for header in self.headers.get_all('Content-Disposition'):
                if 'filename=' in header:
                    filename = header.split('filename=')[1].strip('"')
                    break
            if filename is None:
                self.wfile.write(b'ERROR: No filename specified in upload request.')
                return

            # Create the upload directory if it doesn't exist
            if not os.path.exists(UPLOAD_DIRECTORY):
                os.makedirs(UPLOAD_DIRECTORY)

            # Save the firmware file to the server's file system
            filepath = os.path.join(UPLOAD_DIRECTORY, filename)
            with open(filepath, "wb") as firmware_file:
                firmware_data = self.rfile.read(content_length)
                firmware_file.write(firmware_data)
            firmware_update = 1
#             self.wfile.write(b'firmware uploaded to server successfully\nfirmware update available')

def main():
    httpd = HTTPServer(server_address, MyHandler)
    print("Server running on port", PORT)
    httpd.serve_forever()

if __name__ == '__main__':
    main()
