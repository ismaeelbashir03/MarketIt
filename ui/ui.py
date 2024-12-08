import tkinter as tk
from tkinter import ttk, messagebox
import requests
import json

# Server base URL
BASE_URL = "http://127.0.0.1:18080"

def update_output(message):
    """Update the output text in the GUI."""
    output_text.config(state="normal")
    output_text.delete("1.0", tk.END)
    output_text.insert(tk.END, message)
    output_text.config(state="disabled")
    output_text.see(tk.END)  # Automatically scroll to the bottom

def send_get_events():
    try:
        response = requests.get(f"{BASE_URL}/api/events")
        if response.status_code == 200:
            update_output(f"Events:\n{response.json()}")
        else:
            update_output(f"Failed to get events: {response.status_code}")
    except Exception as e:
        update_output(f"Error: {str(e)}")

def send_post_event():
    try:
        description = event_description.get()
        data = {"description": description}
        response = requests.post(f"{BASE_URL}/api/events", json=data)
        if response.status_code == 201:
            update_output("Event created successfully")
        else:
            update_output(f"Failed to create event: {response.status_code}")
    except Exception as e:
        update_output(f"Error: {str(e)}")

def send_get_user():
    try:
        user_id = user_id_input.get()
        data = {"id": int(user_id)}
        response = requests.get(f"{BASE_URL}/api/user", json=data)
        if response.status_code == 200:
            update_output(f"User:\n{response.json()}")
        else:
            update_output(f"Failed to get user: {response.status_code}")
    except Exception as e:
        update_output(f"Error: {str(e)}")

def send_post_user():
    try:
        user_id = user_id_input.get()
        name = user_name_input.get()
        data = {"id": int(user_id), "name": name}
        response = requests.post(f"{BASE_URL}/api/user", json=data)
        if response.status_code == 201:
            update_output("User created successfully")
        else:
            update_output(f"Failed to create user: {response.status_code}")
    except Exception as e:
        update_output(f"Error: {str(e)}")

def send_post_order():
    """Send order creation request."""
    try:
        user_id = user_id_order_input.get()
        price = float(order_price_input.get())
        quantity = int(order_quantity_input.get())
        side = side_var.get()  # 0 for BUY, 1 for SELL
        order_type = order_type_var.get()  # 0 for LIMIT, 1 for MARKET
        event_id = int(event_id_input.get())  # New event_id field

        # Create order payload
        data = {
            "user_id": int(user_id),
            "price": price,
            "quantity": quantity,
            "side": int(side),
            "orderType": int(order_type),
            "event_id": event_id
        }

        # Send POST request
        response = requests.post(f"{BASE_URL}/api/order", json=data)

        if response.status_code == 201:
            update_output("Order created successfully")
        else:
            update_output(f"Failed to create order: {response.status_code}, {response.text}")
    except Exception as e:
        update_output(f"Error: {str(e)}")


# Create the main GUI window
root = tk.Tk()
root.title("API Test GUI")
root.geometry("1080x720")

# Event Management
event_frame = ttk.LabelFrame(root, text="Events")
event_frame.pack(fill="x", padx=10, pady=5)
ttk.Label(event_frame, text="Description:").grid(row=0, column=0, padx=5, pady=5, sticky="e")
event_description = ttk.Entry(event_frame, width=40)
event_description.grid(row=0, column=1, padx=5, pady=5)
ttk.Button(event_frame, text="Get Events", command=send_get_events).grid(row=1, column=0, padx=5, pady=5)
ttk.Button(event_frame, text="Create Event", command=send_post_event).grid(row=1, column=1, padx=5, pady=5)

# User Management
user_frame = ttk.LabelFrame(root, text="Users")
user_frame.pack(fill="x", padx=10, pady=5)
ttk.Label(user_frame, text="User ID:").grid(row=0, column=0, padx=5, pady=5, sticky="e")
user_id_input = ttk.Entry(user_frame, width=10)
user_id_input.grid(row=0, column=1, padx=5, pady=5)
ttk.Label(user_frame, text="Name:").grid(row=1, column=0, padx=5, pady=5, sticky="e")
user_name_input = ttk.Entry(user_frame, width=20)
user_name_input.grid(row=1, column=1, padx=5, pady=5)
ttk.Button(user_frame, text="Get User", command=send_get_user).grid(row=2, column=0, padx=5, pady=5)
ttk.Button(user_frame, text="Create User", command=send_post_user).grid(row=2, column=1, padx=5, pady=5)

# User Management
user_frame = ttk.LabelFrame(root, text="Users")
user_frame.pack(fill="x", padx=10, pady=5)
ttk.Label(user_frame, text="User ID:").grid(row=0, column=0, padx=5, pady=5, sticky="e")
user_id_input = ttk.Entry(user_frame, width=10)
user_id_input.grid(row=0, column=1, padx=5, pady=5)
ttk.Label(user_frame, text="Name:").grid(row=1, column=0, padx=5, pady=5, sticky="e")
user_name_input = ttk.Entry(user_frame, width=20)
user_name_input.grid(row=1, column=1, padx=5, pady=5)
ttk.Button(user_frame, text="Get User", command=send_get_user).grid(row=2, column=0, padx=5, pady=5)
ttk.Button(user_frame, text="Create User", command=send_post_user).grid(row=2, column=1, padx=5, pady=5)

# Order Management Frame
order_frame = ttk.LabelFrame(root, text="Create Order")
order_frame.pack(fill="x", padx=10, pady=5)

# User ID
ttk.Label(order_frame, text="User ID:").grid(row=0, column=0, padx=5, pady=5, sticky="e")
user_id_order_input = ttk.Entry(order_frame, width=20)
user_id_order_input.grid(row=0, column=1, padx=5, pady=5)

# Price
ttk.Label(order_frame, text="Price:").grid(row=1, column=0, padx=5, pady=5, sticky="e")
order_price_input = ttk.Entry(order_frame, width=20)
order_price_input.grid(row=1, column=1, padx=5, pady=5)

# Quantity
ttk.Label(order_frame, text="Quantity:").grid(row=2, column=0, padx=5, pady=5, sticky="e")
order_quantity_input = ttk.Entry(order_frame, width=20)
order_quantity_input.grid(row=2, column=1, padx=5, pady=5)

# Side (Enum)
ttk.Label(order_frame, text="Side:").grid(row=3, column=0, padx=5, pady=5, sticky="e")
side_var = tk.StringVar(value="0")  # Default to "BUY"
side_dropdown = ttk.OptionMenu(order_frame, side_var, "0", "0", "1")
side_dropdown.grid(row=3, column=1, padx=5, pady=5)

# Order Type (Enum)
ttk.Label(order_frame, text="Order Type:").grid(row=4, column=0, padx=5, pady=5, sticky="e")
order_type_var = tk.StringVar(value="0")  # Default to "LIMIT"
order_type_dropdown = ttk.OptionMenu(order_frame, order_type_var, "0", "0 (LIMIT)", "1 (MARKET)")
order_type_dropdown.grid(row=4, column=1, padx=5, pady=5)

# Event ID
ttk.Label(order_frame, text="Event ID:").grid(row=5, column=0, padx=5, pady=5, sticky="e")
event_id_input = ttk.Entry(order_frame, width=20)
event_id_input.grid(row=5, column=1, padx=5, pady=5)

# Submit Button
ttk.Button(order_frame, text="Create Order", command=send_post_order).grid(row=6, column=1, padx=5, pady=5)


# Output with Scrollbar
output_frame = ttk.LabelFrame(root, text="Output")
output_frame.pack(fill="x", padx=10, pady=5)
output_scrollbar = ttk.Scrollbar(output_frame)
output_scrollbar.pack(side="right", fill="y")
output_text = tk.Text(output_frame, height=10, wrap="word", state="disabled", yscrollcommand=output_scrollbar.set)
output_text.pack(side="left", fill="both", expand=True)
output_scrollbar.config(command=output_text.yview)

# Run the GUI loop
root.mainloop()