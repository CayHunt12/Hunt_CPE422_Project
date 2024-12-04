import csv

def read_temperature_data(file_path):
    """Reads the temperature data from a CSV file and returns a list of temperatures."""
    temperatures = []
    
    try:
        with open(file_path, 'r') as file:
            reader = csv.reader(file)
            for row in reader:
                # Assuming temperature data is in the first column
                try:
                    temp = float(row[0])
                    temperatures.append(temp)
                except ValueError:
                    print(f"Skipping invalid data: {row}")
        return temperatures
    except FileNotFoundError:
        print(f"Error: File '{file_path}' not found.")
        return []

def generate_temperature_report(temperatures):
    """Generates a report on the minimum, maximum, and average temperatures."""
    if not temperatures:
        print("No temperature data available.")
        return
    
    min_temp = min(temperatures)
    max_temp = max(temperatures)
    avg_temp = sum(temperatures) / len(temperatures)

    print("Temperature Report")
    print("====================")
    print(f"Minimum Temperature: {min_temp:.2f} °C")
    print(f"Maximum Temperature: {max_temp:.2f} °C")
    print(f"Average Temperature: {avg_temp:.2f} °C")

if __name__ == "__main__":
    # Replace with your actual file path
    file_path = "temperature_data.csv"  # Example data file
    
    # Read the temperature data from the file
    temperatures = read_temperature_data(file_path)
    
    # Generate and print the report
    generate_temperature_report(temperatures)
