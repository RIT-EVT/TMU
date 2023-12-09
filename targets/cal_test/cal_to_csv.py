import serial
import csv

# Define the serial port settings
ser = serial.Serial('COM10', 9600)  # Change 'COMx' to the appropriate port for your system

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

    except KeyboardInterrupt:
        # Close the serial port when the script is interrupted
        ser.close()
