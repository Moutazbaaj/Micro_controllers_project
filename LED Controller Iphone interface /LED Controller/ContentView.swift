//
//  ContentView.swift
//  LED Controller
//
//  Created by Moutaz Baaj on 07.10.24.
//

import SwiftUI
import CoreBluetooth

struct ContentView: View {
    @ObservedObject var bluetoothManager = BluetoothManager()

    var body: some View {
        VStack(spacing: 20) {
            Button("Turn On All Lights") {
                bluetoothManager.sendCommand("1")
            }
            .buttonStyle(.bordered)
            Button("Turn Off All Lights") {
                bluetoothManager.sendCommand("0")
            }
            .buttonStyle(.bordered)
            Button("Toggle Red Light") {
                bluetoothManager.sendCommand("r")
            }
            .buttonStyle(.bordered)
            Button("Toggle Green Light") {
                bluetoothManager.sendCommand("g")
            }
            .buttonStyle(.bordered)
            Button("Toggle Blue Light") {
                bluetoothManager.sendCommand("b")
            }
            .buttonStyle(.bordered)
            Button("Toggle Yellow Light") {
                bluetoothManager.sendCommand("y")
            }
            .buttonStyle(.bordered)
            Button("Toggle White Light") {
                bluetoothManager.sendCommand("w")
            }
            .buttonStyle(.bordered)
        }
        .padding()
    }
}

class BluetoothManager: NSObject, ObservableObject, CBCentralManagerDelegate, CBPeripheralDelegate {
    var centralManager: CBCentralManager!
    var hm19Peripheral: CBPeripheral!
    var hm19Characteristic: CBCharacteristic?

    override init() {
        super.init()
        centralManager = CBCentralManager(delegate: self, queue: nil)
    }

    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        print("Bluetooth state updated: \(central.state)")
        if central.state == .poweredOn {
            centralManager.scanForPeripherals(withServices: nil, options: nil)
            print("Scanning for DSD TECH...")
        } else {
            print("Bluetooth not available.")
        }
    }

    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {
        print("Discovered peripheral: \(peripheral.name ?? "Unknown")")
        if peripheral.name == "DSD TECH" {
            hm19Peripheral = peripheral
            centralManager.stopScan()
            centralManager.connect(peripheral, options: nil)
            print("Connecting to DSD TECH...")
        }
    }

    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        peripheral.delegate = self
        peripheral.discoverServices(nil)
    }

    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        guard let services = peripheral.services else { return }
        for service in services {
            peripheral.discoverCharacteristics(nil, for: service)
        }
    }

    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        guard let characteristics = service.characteristics else { return }
        for characteristic in characteristics {
            hm19Characteristic = characteristic
        }
    }

    func sendCommand(_ command: String) {
        guard let characteristic = hm19Characteristic else { return }
        if let data = command.data(using: .utf8) {
            hm19Peripheral.writeValue(data, for: characteristic, type: .withResponse)
        }
    }
} 

#Preview {
    ContentView()
}
