/*-----------------------------------------------
DYNOTEST INI HANYA MENGGUNAKAN SATU SENSOR
YAKNI SENSOR ENCODER PUTARAN ROLLER
-----------------------------------------------*/
// Konfigurasi pin sensor RPM
const int sensorPin = 2;  // Pin sensor hall effect atau encoder
volatile int rpmCount = 0;  // Hitungan RPM
unsigned long previousTime = 0;
unsigned long currentTime;
float rpm = 0;             // Nilai RPM
float previousRPM = 0;      // Nilai RPM sebelumnya
float omega = 0;            // Kecepatan sudut (rad/s)
float previousOmega = 0;    // Kecepatan sudut sebelumnya
float alpha = 0;            // Percepatan sudut (rad/s²)
float torque = 0;           // Torsi (Nm)
float powerWatt = 0;        // Daya dalam watt (W)
float powerHP = 0;          // Daya dalam horsepower (HP)
const float inertia = 0.0625; // Momen inersia (kg·m²) untuk roller silinder pejal
const float pi = 3.14159265359;

// Fungsi interrupt untuk menghitung putaran
void rpmCounter() {
  rpmCount++;
}

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(sensorPin), rpmCounter, RISING);
  
  // Inisialisasi waktu sebelumnya
  previousTime = millis();
}

void loop() {
  currentTime = millis();
  
  // Hitung waktu yang telah berlalu setiap 1 detik
  if (currentTime - previousTime >= 1000) {
    detachInterrupt(digitalPinToInterrupt(sensorPin));  // Matikan interrupt sementara untuk hitungan stabil
    
    // Hitung RPM
    rpm = (rpmCount / 20.0) * 60;  // Asumsikan 20 pulsa per putaran (sesuaikan dengan sensor)
    
    // Reset counter RPM
    rpmCount = 0;
    
    // Hitung kecepatan sudut (ω) dalam radian per detik
    omega = (rpm * 2 * pi) / 60;
    
    // Hitung percepatan sudut (α) jika ada perubahan kecepatan
    alpha = (omega - previousOmega) / ((currentTime - previousTime) / 1000.0); // Perubahan kecepatan sudut per waktu
    
    // Hitung torsi (τ)
    torque = inertia * alpha;

    // Hitung daya dalam watt (P = τ * ω)
    powerWatt = torque * omega;

    // Konversi daya ke horsepower (1 HP = 745.7 W)
    powerHP = powerWatt / 745.7;

    // Tampilkan hasil di serial monitor
    Serial.print("RPM: ");
    Serial.println(rpm);
    Serial.print("Kecepatan Sudut (ω): ");
    Serial.print(omega);
    Serial.println(" rad/s");
    Serial.print("Percepatan Sudut (α): ");
    Serial.print(alpha);
    Serial.println(" rad/s²");
    Serial.print("Torsi (τ): ");
    Serial.print(torque);
    Serial.println(" Nm");
    Serial.print("Daya (Power): ");
    Serial.print(powerWatt);
    Serial.println(" W");
    Serial.print("Daya (Power): ");
    Serial.print(powerHP);
    Serial.println(" HP");

    // Update nilai sebelumnya
    previousRPM = rpm;
    previousOmega = omega;
    
    // Reset waktu sebelumnya
    previousTime = currentTime;
    
    // Aktifkan kembali interrupt
    attachInterrupt(digitalPinToInterrupt(sensorPin), rpmCounter, RISING);
  }
}
