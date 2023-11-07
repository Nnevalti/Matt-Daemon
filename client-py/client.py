import tkinter as tk
import tkinter.messagebox as messagebox
import socket
import ssl
import signal
import threading
import time
from Spinner import Spinner

def send_message(client_socket, message):
    try:
        client_socket.send((message + '\n').encode())
    except Exception as e:
        return str(e)

def receive_messages(client_socket, chat_history):
    while True:
        try:
            message = client_socket.recv(1024).decode()
            if not message:
                break
            chat_history.config(state=tk.NORMAL)
            chat_history.insert(tk.END, f'{message}\n')
            chat_history.config(state=tk.DISABLED)
        except Exception as e:
            messagebox.showerror("Error", f"Error receiving message: {e}")
            break

def signal_handler(signal, frame):
    print("\rEnding connection ", end="", flush=True)
    spinner = Spinner(type="dots").start()
    time.sleep(2)
    root.destroy()
    spinner.stop()
    print("\n", end="", flush=True)

context = ssl.create_default_context(ssl.Purpose.SERVER_AUTH)
context.check_hostname = False
context.verify_mode = ssl.CERT_NONE

try:
    server_socket = socket.create_connection(('127.0.0.1', 4243))
    client_socket = context.wrap_socket(server_socket, server_hostname='server')
except Exception as e:
    messagebox.showerror("Error", f"Unable to connect to the server: Are you sure it is up and running ?\n {e}")
    exit(1)

def on_enter(event):
    send_message_handler()

def send_message_handler():
    message = input_field.get().strip()
    if message:
        result = send_message(client_socket, message)
        if result:
            messagebox.showerror("Error", f"Error sending message: {result}")
        else:
            chat_history.config(state=tk.NORMAL)
            chat_history.insert(tk.END, f'You: {message}\n')
            chat_history.config(state=tk.DISABLED)
        input_field.delete(0, tk.END)

root = tk.Tk()
root.title('SSL Chat Client')

chat_history = tk.Text(root, width=40, height=10, state=tk.DISABLED, background='black', foreground='white')
chat_history.pack()

input_field = tk.Entry(root, width=30)
input_field.pack()
input_field.bind("<Return>", on_enter)

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

signal.signal(signal.SIGINT, signal_handler)

root.mainloop()

client_socket.close()
