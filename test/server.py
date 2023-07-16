from http.server import HTTPServer, SimpleHTTPRequestHandler
import subprocess

class RequestHandler(SimpleHTTPRequestHandler):
    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        ip = post_data.decode('utf-8')
        
        ip = ip[ip.find(':') + 2:-2]

        # Ejecutar el archivo .sh pasando la dirección IP como argumento
        subprocess.run(['./script.sh', ip])

        self.send_response(200)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()
        self.wfile.write(b'Archivo .sh ejecutado correctamente')

def run():
    server_address = ('', 8001)
    httpd = HTTPServer(server_address, RequestHandler)
    print('Servidor en ejecución en http://localhost:8001')
    httpd.serve_forever()

if __name__ == '__main__':
    run()
