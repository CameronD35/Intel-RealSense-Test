from rosbags.rosbag1 import Reader
from rosbags.typesys import get_types_from_msg, register_types, Stores, get_typestore
import rosbag
import yaml
import os

# Define the custom message type - StreamInfo
STREAMINFO_MSG_DEF = """
std_msgs/Header header
uint32 fps
uint32 width
uint32 height
string encoding
string format
uint32 width_step
"""

# Create a type store with custom types
typestore = get_typestore(Stores.ROS2_FOXY)

# Register the custom message type
custom_types = get_types_from_msg(STREAMINFO_MSG_DEF, 'realsense_msgs/msg/StreamInfo')
register_types(custom_types)

file_path = "./bagFiles/testfile.bag" # Change on your end

def print_message(msg):
    if hasattr(msg, '__dict__'):
        for key, value in msg.__dict__.items():
            print(f"  {key}: {value}")
    else:
        print(f"  {msg}")

print(f"Opening bag file: {file_path}")

try:
    with Reader(file_path) as reader:
        connections = [x for x in reader.connections]
        print(f"Found {len(connections)} topics")
        
        for connection in connections:
            if connection.topic != '/file_version':
                print(f"\nReading Topic: {connection.topic}")
                print(f"Message type: {connection.msgtype}")
                
                count = 0
                for entry in reader.messages([connection]):
                    topic, timestamp, rawdata = entry
                    try:
                        # Use the typestore directly for deserialization
                        msg = typestore.deserialize_cdr(rawdata, connection.msgtype)
                        print(f"Message {count}:")
                        print_message(msg)
                    except KeyError as e:
                        print(f"  Error: Could not deserialize message of type {connection.msgtype}")
                        print(f"  KeyError: {e}")
                    except Exception as e:
                        print(f"  Error: {e}")
                    
                    count += 1
                    if count >= 3:  # Limit to 3 messages per topic for brevity
                        print(f"  ... (showing only first 3 messages of {connection.topic})")
                        break
            else:
                print("\nSkipping /file_version topic")
                
        print("\nSummary of topics in bag file:")
        for i, conn in enumerate(connections):
            print(f"{i+1}. {conn.topic} ({conn.msgtype})")

except FileNotFoundError:
    print(f"Error: Bag file not found at {file_path}")
    print(f"Current working directory: {os.getcwd()}")
except Exception as e:
    print(f"Error opening bag file: {e}")