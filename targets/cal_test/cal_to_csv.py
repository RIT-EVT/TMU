import serial
import csv
import time

# Open the serial port
ser = serial.Serial('COM10', 9600)  # Replace the COM port with the one you're using

# Open a CSV file for writing
csv_file_path = 'sensor_data.csv'
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
            temps = [int(value.split(':')[1]) for value in values[:4]]
            errors = [int(value.split(':')[1]) for value in values[4:]]

            # Write the values to the CSV file
            csv_writer.writerow(temps + errors)

            # Flush the buffer to update the file in real-time
            csv_file.flush()

            # Wait 1 second before reading the next line
            time.sleep(1)

    except KeyboardInterrupt:
        # Close the serial port when the script is interrupted
        ser.close()
