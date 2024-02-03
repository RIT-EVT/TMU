import serial
import csv
import time

# Open the serial port
com_port = input('Enter the COM port (Format: COMXX): ')
ser = serial.Serial(com_port, 9600)  # Replace the COM port with the one you're using

try:
    # Generate a new file name with timestamp
    file_name = f'sensor_data_{time.strftime("%Y%m%d-%H%M%S")}.csv'
    csv_file_path = f'./{file_name}'


    # Open a new CSV file for writing
    with open(csv_file_path, 'w', newline='') as csv_file:
        csv_writer = csv.writer(csv_file)

        # Write the header to the CSV file
        csv_writer.writerow(['Temp1', 'Temp2', 'Temp3', 'Temp4', 'Error1', 'Error2', 'Error3', 'Error4'])

        try:
            while True:
                # Read a line from the serial port
                line = ser.readline().decode('utf-8').strip()

                # Split the line into individual values
                values = line.split(',')

                temps = [int(value) for value in values[:4]]
                errors = [int(value) for value in values[4:]]

                # Write the values to the CSV file
                csv_writer.writerow(temps + errors)

                # Flush the buffer to update the file in real-time
                csv_file.flush()

                # Wait 1 second before reading the next line
                time.sleep(1)

        except KeyboardInterrupt:
            # Close the serial port when the script is interrupted
            ser.close()

except Exception as e:
    print(f"An error occurred: {e}")

finally:
    # Close the serial port when the script is finished
    ser.close()
