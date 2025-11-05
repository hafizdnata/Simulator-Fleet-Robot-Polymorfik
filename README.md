Fleet Robot Simulation Project (C++ OOP Polymorphism)
=====================================================

Proyek ini merupakan simulasi armada kendaraan robotik yang terdiri dari berbagai jenis kendaraan seperti GroundRobot, AerialDrone, dan Amphibious. Program ini menampilkan penerapan prinsip Object-Oriented Programming (OOP) dalam C++, termasuk pewarisan, overriding, overloading, dan penggunaan std::unique_ptr untuk pengelolaan memori yang aman.

Setiap kendaraan memiliki perilaku pergerakan yang berbeda dan dikendalikan secara polimorfik melalui kelas dasar Vehicle. Proyek ini dikembangkan dalam kelompok dengan pembagian peran dan tanggung jawab yang jelas untuk memastikan semua anggota berkontribusi.

-----------------------------------------------------
Pembagian Tugas Kelompok
-----------------------------------------------------

+-------------+-------------------------------------------------------------+
|  Anggota    | Tugas dan Tanggung Jawab                                    |
+-------------+-------------------------------------------------------------+
| Hafizh      | Desain dan implementasi Vehicle base class, termasuk        |
|             | Position struct dan fungsi clone()                          |
+-------------+-------------------------------------------------------------+
| Judhis      | Implementasi GroundRobot serta sistem obstacles dengan      |
|             | metode override move() dan obstacleAhead()                 |
+-------------+-------------------------------------------------------------+
| Octa        | Implementasi AerialDrone, termasuk status reporting dan     |
|             | overloaded move()                                           |
+-------------+-------------------------------------------------------------+
| Dion        | Pengembangan Amphibious (integration) dan Fleet Manager     |
|             | yang menggunakan vector dan fungsi executeMove()            |
+-------------+-------------------------------------------------------------+
| Zahra       | Bagian Testing, pembuatan scenario scripts, serta           |
|             | dokumentasi (README, test cases, dan demo run)              |
+-------------+-------------------------------------------------------------+

-----------------------------------------------------
Cara Kompilasi (Universal)
-----------------------------------------------------

Proyek ini dapat dijalankan pada Windows, Linux, dan macOS menggunakan compiler C++17.
Pastikan file utama bernama robot.cpp (atau sesuaikan dengan nama file program utama Anda).

1. Windows
   Buka Command Prompt, lalu jalankan perintah:
   g++ -std=c++17 -O2 -o fleet_sim.exe robot.cpp

2. Linux
   Buka Terminal, lalu jalankan perintah:
   g++ -std=c++17 -O2 -o fleet_sim robot.cpp

3. macOS
   Gunakan Terminal, lalu jalankan perintah:
   clang++ -std=c++17 -O2 -o fleet_sim robot.cpp

-----------------------------------------------------
Cara Menjalankan Program
-----------------------------------------------------

Setelah dikompilasi, jalankan program sesuai sistem operasi Anda.

- Windows:
  fleet_sim.exe

- Linux / macOS:
  ./fleet_sim

Program akan menampilkan simulasi perjalanan beberapa kendaraan melalui waypoint dengan mencatat posisi, mode, dan status baterai.

-----------------------------------------------------
Contoh File scenarios.txt
-----------------------------------------------------

WAYPOINT 1: (2.0, 1.0, 0.0)
WAYPOINT 2: (5.0, 3.0, 2.0)
WAYPOINT 3: (8.0, 0.0, 0.0)

File ini digunakan untuk mengatur urutan pergerakan seluruh kendaraan dalam simulasi.

-----------------------------------------------------
Contoh Output Program
-----------------------------------------------------

Fleet Robot Simulator (mini)

=== Fleet status ===
[GR1] pos=(0,0,0) battery=50.0%
[AD1] Drone pos=(0,0,5) altitude=5 battery=80.0%
[AMP1] Amphibious pos=(0,-2,0) mode=GROUND battery=90.0%
====================

-- Waypoint 1: (2.0,1.0,0.0)
>>> Perintah ke GR1:
[GR1] ground bergerak ke (2,1)
>>> Perintah ke AD1:
[AD1] drone bergerak ke (2,1,0)
>>> Perintah ke AMP1:
[AMP1] Amphibious in GROUND mode.
[AMP1] ground bergerak ke (2,1)
...

-----------------------------------------------------
Deskripsi Singkat
-----------------------------------------------------

Kelas Vehicle menjadi dasar seluruh kendaraan dengan atribut posisi (Position), ID, dan kapasitas baterai.  
GroundRobot beroperasi di darat, mendeteksi rintangan, dan membatasi kemiringan medan.
AerialDrone bergerak di udara dengan perhitungan 3D dan memiliki status ketinggian.
Amphibious menggabungkan keduanya dan dapat berpindah mode otomatis antara darat dan udara.

Semua kendaraan dikelola dalam std::vector<std::unique_ptr<Vehicle>> yang memungkinkan pengelolaan objek secara polimorfik.

-----------------------------------------------------
Konsep OOP yang Diterapkan
-----------------------------------------------------

- Abstraction & Inheritance : Vehicle sebagai kelas dasar abstrak.
- Polymorphism              : Fungsi virtual move, status, dan clone.
- Overloading & Overriding  : Fungsi move dengan parameter berbeda dan implementasi unik di setiap turunan.
- Encapsulation             : Atribut dilindungi (protected) dan diakses melalui metode turunan.
- Smart Pointer             : Menggunakan std::unique_ptr untuk manajemen memori otomatis.

-----------------------------------------------------
Struktur Folder (Opsional)
-----------------------------------------------------

project/
│
├── src/
│   ├── vehicle.h
│   ├── ground_robot.h
│   ├── aerial_drone.h
│   ├── amphibious.h
│   └── robot.cpp
│
├── scenarios/
│   └── scenarios.txt
│
└── README.txt

-----------------------------------------------------
Catatan
-----------------------------------------------------

Proyek ini merupakan hasil kerja tim dalam menerapkan konsep Polymorphism, Inheritance, dan Virtual Function di C++.
Tidak memerlukan dependensi tambahan selain compiler standar C++17.
Struktur kode dan dokumentasi disusun agar mudah dikembangkan serta diuji pada berbagai platform.

-----------------------------------------------------
TEKNIK ROBOTIKA DAN KECERDASAN BUATAN, UNIVERSITAS AIRLANGGA-Fleet Robot Simulation Project
-----------------------------------------------------
