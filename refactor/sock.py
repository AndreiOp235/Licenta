import asyncio

HOST = '192.168.50.192'  # Change if needed
PORT = 6000

async def handle_client(reader, writer):
    addr = writer.get_extra_info('peername')
    print(f"Connection from {addr}")

    try:
        data = await reader.readline()
        message = data.decode().strip()
        print(f"Received MAC: {message}")
    except Exception as e:
        print(f"Error handling client: {e}")
    finally:
        writer.close()
        await writer.wait_closed()
        print(f"Closed connection from {addr}")

async def main():
    server = await asyncio.start_server(handle_client, HOST, PORT)
    addr = server.sockets[0].getsockname()
    print(f"Server listening on {addr}")

    async with server:
        await server.serve_forever()

if __name__ == '__main__':
    asyncio.run(main())
