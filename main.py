import socket
import serial

# Serial port settings
SERIAL_PORT = "COM12"
BAUD_RATE = 115200

# Robot's server address and port
HOST = "127.0.0.1"
PORT = 5511

def main():
    # Open serial connection
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Connected to serial port {SERIAL_PORT} at {BAUD_RATE} baud")
    except Exception as e:
        print(f"Error opening serial port: {e}")
        return

    # Create a socket object
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Connect to the robot controller's socket server
        client_socket.connect((HOST, PORT))
        print(f"Connected to {HOST}:{PORT}")

        # Receive the initial message from the robot
        data = client_socket.recv(1024)  # Buffer size of 1024 bytes
        if data:
            print(f"Received from robot: {data.decode('utf-8')}")

        # Read from serial and send to the robot
        while True:
            if ser.in_waiting > 0:
                serial_data = ser.readline().decode('utf-8').strip()
                print(f"Serial Received: {serial_data}")
                
                if serial_data:
                    client_socket.sendall(serial_data.encode('utf-8'))
                    print(f"Sent to robot: {serial_data}")

    except Exception as e:
        print(f"Error: {e}")

    finally:
        # Close connections
        ser.close()
        client_socket.close()
        print("Connections closed")

if __name__ == "__main__":
    main()
