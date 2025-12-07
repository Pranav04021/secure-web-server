const http = require('http');
http.createServer((req, res) => {
  res.end("Backend server running!");
}).listen(3000);
