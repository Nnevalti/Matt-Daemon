import tkinter as tk
import tkinter.messagebox as messagebox
import socket
import ssl
import signal
import threading
import time
from Spinner import Spinner

# Define the server IP address
SERVER_IP = '127.0.0.1'
SERVER_PORT = 4243

def send_message(client_socket, message):
    try:
        client_socket.send((message + '\n').encode())
    except Exception as e:
        print(f"Error sending message: {e}")
        messagebox.showerror("Error", f"Error sending message: {e}")

def receive_messages(client_socket, chat_history):
    while True:
        try:
            message = client_socket.recv(1024).decode()
            if not message:
                print("Server disconnected")
                status_label.config(bg="red")
                break
            chat_history.config(state=tk.NORMAL)
            chat_history.insert(tk.END, f"{message}")
            chat_history.config(state=tk.DISABLED)
        except Exception as e:
            print(f"Error receiving message: {e}")
            messagebox.showerror("Error", f"Error receiving message: {e}")
            break

def signal_handler(signal, frame):
    print("\rEnding connection ", end="", flush=True)
    spinner = Spinner(type="dots").start()
    time.sleep(2)
    root.destroy()
    spinner.stop()
    print("\n", end="", flush=True)

def send_message_handler():
    message = input_field.get().strip()
    if message:
        result = send_message(client_socket, message)
        if not result:
            chat_history.config(state=tk.NORMAL)
            chat_history.insert(tk.END, f'You: {message}\n')
            chat_history.config(state=tk.DISABLED)
        input_field.delete(0, tk.END)

context = ssl.create_default_context(ssl.Purpose.SERVER_AUTH)
context.check_hostname = False
context.verify_mode = ssl.CERT_NONE

try:
    server_socket = socket.create_connection((SERVER_IP, SERVER_PORT))
    client_socket = context.wrap_socket(server_socket, server_hostname='server')
except Exception as e:
    print(f"Error connecting to the server: {e}")
    messagebox.showerror("Error", f"Unable to connect to the server:\nAre you sure it is up and running ?\n\nError: {e}")
    exit(1)

root = tk.Tk()
root.title('Ben_AFK SSL Chat Client')

status_label = tk.Label(root, text="Status", bg="green", width=10)
status_label.pack()

chat_history = tk.Text(root, width=40, height=10, state=tk.DISABLED, background='black', foreground='white')
chat_history.pack()

input_field = tk.Entry(root, width=30)
input_field.pack()

def on_enter(event):
    send_message_handler()

input_field.bind("<Return>", on_enter)
input_field.bind("<KP_Enter>", on_enter)

button_frame = tk.Frame(root)
button_frame.pack()

send_button = tk.Button(button_frame, text="Send", command=send_message_handler)
send_button.pack(side=tk.LEFT)

def quit_handler():
    send_message(client_socket, "quit")
    root.destroy()

quit_button = tk.Button(button_frame, text="Quit", command=quit_handler)
quit_button.pack(side=tk.LEFT)

receive_thread = threading.Thread(target=receive_messages, args=(client_socket, chat_history))
receive_thread.daemon = True
receive_thread.start()

status_label.config(bg="green")

signal.signal(signal.SIGINT, signal_handler)

root.mainloop()

client_socket.close()
