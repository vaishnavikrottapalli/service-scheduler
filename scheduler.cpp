#include <bits/stdc++.h>
using namespace std;

class Appointment
{
    public:
        int id;
        int operatorID;
        int startTime;
        int endTime;
        string customerName;
};

class ServiceOperator {
    public:
        int id;
        vector<Appointment> appointments;
};

vector<ServiceOperator> serviceOperators;
int appointmentIdCounter = 0;

void bookAppointment(int operatorId, int startTime, int endTime, const string& customerName) {
    // Check if the operator exists
    if (operatorId < 0 || operatorId >= serviceOperators.size()) {
        cout << "Invalid operator ID." << endl;
        return;
    }

    // Check if the time slot is available
    ServiceOperator& operatorData = serviceOperators[operatorId];
    for (const Appointment& appointment : operatorData.appointments) {
        if (startTime < appointment.endTime && endTime > appointment.startTime) {
            cout << "Appointment slot is already booked." << endl;
            return;
        }
    }

    // Book the appointment
    Appointment newAppointment;
    newAppointment.id = appointmentIdCounter++;
    newAppointment.operatorID = operatorId;
    newAppointment.startTime = startTime;
    newAppointment.endTime = endTime;
    newAppointment.customerName = customerName;

    operatorData.appointments.push_back(newAppointment);
    cout << "Appointment booked successfully. your Booking ID is: " << newAppointment.id << endl;
}

void displayAppointments(int operatorId) {
    if (operatorId < 0 || operatorId >= serviceOperators.size()) {
        cout << "Invalid operator ID." << endl;
        return;
    }

    ServiceOperator& operatorData = serviceOperators[operatorId];
    cout << "Appointments for Operator " << operatorId << ":\n";
    for (const Appointment& appointment : operatorData.appointments) {
        cout << "ID: " << appointment.id << ", Customer: " << appointment.customerName
                  << ", Start Time: " << appointment.startTime << ", End Time: " << appointment.endTime << endl;
    }
}

void displayAvailableSlots(int operatorId)
{
    if (operatorId < 0 || operatorId >= serviceOperators.size()) {
        cout << "Invalid operator ID." << endl;
        return;
    }

    ServiceOperator& operatorData = serviceOperators[operatorId];

    cout << "Available slots in Service Operator " << operatorId << ":\n";
    vector<pair<int, int>> mergedSlots;

    mergedSlots.push_back(make_pair(0, 24));

    for (const Appointment& appointment : operatorData.appointments) {
        vector<pair<int, int>> newMergedSlots;

        for (const pair<int, int>& slot : mergedSlots) {

            // If the appointment overlaps with the current slot, split it into multiple slots

            if (appointment.startTime < slot.second && appointment.endTime > slot.first) {
                if (appointment.startTime > slot.first) {
                    newMergedSlots.push_back(make_pair(slot.first, appointment.startTime));
                }
                if (appointment.endTime < slot.second) {
                    newMergedSlots.push_back(make_pair(appointment.endTime, slot.second));
                }
            } else {
                newMergedSlots.push_back(slot);
            }
        }

        mergedSlots = newMergedSlots;
    }

    // Display the available slots in merged way
    for (const auto& slot : mergedSlots) {
        if (slot.first != slot.second) {
            cout << slot.first << "-" << slot.second << endl;
        }
    }

}

void cancelAppointment(int operatorId, int appointmentId) {
    // Check if the operator exists
    if (operatorId < 0 || operatorId >= serviceOperators.size()) {
        cout << "Invalid operator ID." << endl;
        return;
    }

    ServiceOperator& operatorData = serviceOperators[operatorId];

    // Find the appointment with the given ID
    auto appointmentIt = find_if(operatorData.appointments.begin(), operatorData.appointments.end(),
        [appointmentId](const Appointment& appointment) { return appointment.id == appointmentId; });

    if (appointmentIt != operatorData.appointments.end()) {
        // Remove the appointment from the operator's list
        operatorData.appointments.erase(appointmentIt);
        cout << "Appointment canceled successfully." << endl;
    } else {
        cout << "Appointment not found." << endl;
    }
}

void rescheduleAppointment(int operatorId, int appointmentId, int newStartTime, int newEndTime) {
    // Check if the operator exists
    if (operatorId < 0 || operatorId >= serviceOperators.size()) {
        cout << "Invalid operator ID." << endl;
        return;
    }

    ServiceOperator& operatorData = serviceOperators[operatorId];

    // Find the appointment with the given ID
    auto appointmentIt = find_if(operatorData.appointments.begin(), operatorData.appointments.end(),
        [appointmentId](const Appointment& appointment) { return appointment.id == appointmentId; });

    if (appointmentIt != operatorData.appointments.end()) {
        // Check if the new time slots are available
        int newAppointmentId = appointmentIt->id; // Keep the same ID

        for (const Appointment& appointment : operatorData.appointments) {
            if (newStartTime < appointment.endTime && newEndTime > appointment.startTime &&
                &appointment != &(*appointmentIt)) {
                cout << "New appointment time slots are not available. Rescheduling failed." << endl;
                return;
            }
        }

        // Update the appointment with the desired time slots
        appointmentIt->startTime = newStartTime;
        appointmentIt->endTime = newEndTime;

        cout << "Appointment rescheduled successfully with the same ID: " << newAppointmentId << endl;
    } else {
        cout << "Appointment not found. Rescheduling failed." << endl;
    }
}


int main() {
    // Create three service operators
    for (int i = 0; i < 3; ++i) {
        ServiceOperator operatorData;
        operatorData.id = i;
        serviceOperators.push_back(operatorData);
    }
    int option;
    int oper, startTime, endTime, bookingid;
    string custName;
    cout << "MENU" << endl;
    do
    {
        cout << "  1- New Appointment Booking " << endl;
        cout << "  2- Reschedule Booking " << endl;
        cout << "  3- Cancel Booking " << endl;
        cout << "  4- Display Booked Appointments " << endl;
        cout << "  5- Display Available slots " << endl;
        cout << "  6- Exit to leave " << endl;
        cout << "_______________________________________"<< endl;
        cout << "pick an option >>  ";
        cin >> option;
        cout << endl;

        switch (option)
        {
        case 1:
            cout << "Note: Duration of each slot is 1 hour. "<< endl;
            cout << " please choose your operator: \n 0- Service Operator 0 \n 1- Service Operator 1 \n 2- Service Operator 2 \n  " << endl;
            cin >> oper;
            while(oper > 2 || oper <0 )
            {
                cout << "choose a valid operator from 0,1,2 : ";
                cin >> oper;
            }
            displayAvailableSlots(oper);
            cin.ignore();
            cout << " Enter the name of customer: ";
            getline(cin, custName);
            cout << " enter start time of your slot: ";
            cin >> startTime;
            while(startTime >23 || startTime <0)
            {
                cout << "please choose a valid start time! " << endl;
                cin >> startTime;
            }
            bookAppointment(oper, startTime, startTime+1, custName);
            cout << "*****************************************" << endl;
            break;

        case 2:
            cout << " please choose your operator: \n 0- Service Operator 1 \n 1- Service Operator 2 \n 2- Service Operator 3 \n  " << endl;
            cin >> oper;
            cout << "enter your booking ID : ";
            cin >> bookingid;
            displayAvailableSlots(oper);
            cout << " enter new start time of your slot: ";
            cin >> startTime;
            while(startTime >23 || startTime <0)
            {
                cout << "please choose a valid start time! " << endl;
                cin >> startTime;
            }
            rescheduleAppointment(oper, bookingid, startTime, startTime+1);
            cout << "*****************************************" << endl;
            break;

        case 3:
            cout << " please choose your operator: \n 0- Service Operator 1 \n 1- Service Operator 2 \n 2- Service Operator 3 \n  " << endl;
            cin >> oper;
            cout << "enter your booking ID : ";
            cin >> bookingid;
            cancelAppointment(oper, bookingid);
            cout << "*****************************************" << endl;
            break;


        case 4:
            cout << " please choose your operator whose bookings you want to display: \n 0- Service Operator 1 \n 1- Service Operator 2 \n 2- Service Operator 3 \n  " << endl;
            cin >> oper;
            displayAppointments(oper);
            cout << "*****************************************" << endl;
            break;
        
        case 5:
            cout << " please choose your operator whose availavle slots you want to display : \n 0- Service Operator 1 \n 1- Service Operator 2 \n 2- Service Operator 3 \n  " << endl;
            cin >> oper;
            displayAvailableSlots(oper);
            cout << "*****************************************" << endl;
            break;
        
        default:
            cout << "thanks come back again " << endl;
            break;
        }
    } while (option != 6);
    return 0;
}