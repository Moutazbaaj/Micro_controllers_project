//
//  ContentView.swift
//  LED Controller
//
//  Created by Moutaz Baaj on 07.10.24.
//

import SwiftUI

struct ContentView: View {
    let esp8266Address = "http://192.168.178.153"  // Replace with your ESP8266 IP address
    
    // States for WiFi controls
    @State private var isRedOn = false
    @State private var isGreenOn = false
    @State private var isBlueOn = false
    @State private var isYellowOn = false
    @State private var isWhiteOn = false
    
    var body: some View {
        VStack(spacing: 20) {
            Text("LED Controller")
                .font(.largeTitle)
                .fontWeight(.bold)
                .foregroundColor(.white)
                .padding()
            
            VStack(spacing: 15) {
                ToggleButton(isOn: $isRedOn, color: .red, label: "Red Light", onCommand: "/red/on", offCommand: "/red/off")
                ToggleButton(isOn: $isGreenOn, color: .green, label: "Green Light", onCommand: "/green/on", offCommand: "/green/off")
                ToggleButton(isOn: $isBlueOn, color: .blue, label: "Blue Light", onCommand: "/blue/on", offCommand: "/blue/off")
                ToggleButton(isOn: $isYellowOn, color: .yellow, label: "Yellow Light", onCommand: "/yellow/on", offCommand: "/yellow/off")
                ToggleButton(isOn: $isWhiteOn, color: .white, label: "White Light", onCommand: "/white/on", offCommand: "/white/off")
            }
            .padding()
        }
        .frame(maxWidth: .infinity, maxHeight: .infinity)
        .background(LinearGradient(gradient: Gradient(colors: [Color.purple, Color.blue]), startPoint: .top, endPoint: .bottom))
        .ignoresSafeArea()
    }
    
    // Function to send HTTP commands to ESP8266
    func sendHttpCommand(endpoint: String) {
        guard let url = URL(string: esp8266Address + endpoint) else {
            print("Invalid URL")
            return
        }
        
        var request = URLRequest(url: url)
        request.httpMethod = "GET"
        
        URLSession.shared.dataTask(with: request) { data, response, error in
            if let error = error {
                print("Error: \(error.localizedDescription)")
            } else if let httpResponse = response as? HTTPURLResponse {
                print("HTTP Response: \(httpResponse.statusCode)")
                if let data = data {
                    let responseData = String(data: data, encoding: .utf8) ?? ""
                    print("Response data: \(responseData)")
                }
            }
        }.resume()
    }
}

struct ToggleButton: View {
    @Binding var isOn: Bool
    var color: Color
    var label: String
    var onCommand: String
    var offCommand: String
    
    var body: some View {
        Button(action: {
            isOn.toggle()
            let command = isOn ? onCommand : offCommand
            sendHttpCommand(endpoint: command)
        }) {
            HStack {
                Circle()
                    .fill(isOn ? color : Color.gray)
                    .frame(width: 20, height: 20)
                Text(label)
                    .font(.headline)
                    .foregroundColor(.white)
                    .padding(.leading, 10)
            }
            .frame(maxWidth: .infinity, minHeight: 50)
            .background(RoundedRectangle(cornerRadius: 10).fill(Color.white.opacity(0.1)))
        }
    }
    
    func sendHttpCommand(endpoint: String) {
        guard let url = URL(string: "http://192.168.178.153" + endpoint) else { return }
        
        var request = URLRequest(url: url)
        request.httpMethod = "GET"
        
        URLSession.shared.dataTask(with: request) { _, response, error in
            if let error = error {
                print("Error sending request: \(error)")
            } else if let response = response as? HTTPURLResponse {
                print("HTTP Response: \(response.statusCode)")
            }
        }.resume()
    }
}

#Preview {
    ContentView()
}
