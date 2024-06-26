#include <Ethernet.h>
#include <EthernetUdp.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 0, 100);

unsigned int localPort =8888;      // local port to listen on
char remote_ip[]="192.168.0.101";
unsigned int remote_port=8888;




// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {

  // start the Ethernet
  Ethernet.begin(mac, ip);

  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("some error in hardware.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start UDP
  Udp.begin(localPort);

}

void loop() {
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    Serial.print("You: ");
    Serial.println(message);
    Udp.beginPacket(remote_ip, remote_port); // IP address and port of Arduino 2
    Udp.print(message);
    Udp.endPacket();
  }

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    char packetBuffer[255];
    Udp.read(packetBuffer, 255);
    Serial.print("Rohith:");
    Serial.write(packetBuffer, packetSize);
  

    // send a reply to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.endPacket();
  }
  delay(10);
}
