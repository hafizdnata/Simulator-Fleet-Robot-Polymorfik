# Simulator-Fleet-Robot-Polymorfik
Vehicle Simulation Project (C++ OOP Polymorphism)

Proyek ini merupakan simulasi berbagai jenis kendaraan menggunakan konsep Object-Oriented Programming (OOP) dalam bahasa C++. Sistem ini terdiri dari beberapa kelas turunan yang mewarisi dari kelas dasar (base class) bernama Vehicle. Kelas ini bersifat abstrak dan berisi atribut seperti id, posisi dengan koordinat x, y, z, serta kapasitas baterai. Vehicle juga memiliki fungsi virtual seperti move, status, clone, dan consumeBattery untuk mengatur pergerakan, status, serta konsumsi energi dari setiap kendaraan. 

Kelas GroundRobot merupakan turunan dari Vehicle yang hanya dapat bergerak pada bidang datar, sehingga pergerakan pada sumbu z akan diabaikan. Fungsi move pada kelas ini dioverride agar sesuai dengan batas kemampuan robot darat dan juga dilengkapi dengan fitur obstacleAhead untuk mendeteksi rintangan di jalur pergerakannya. Selain itu, kelas ini memiliki versi overloaded dari fungsi move yang menggunakan heading dan distance sebagai parameter untuk menentukan arah gerak. 

Kelas AerialDrone merupakan turunan lain dari Vehicle yang mampu bergerak secara tiga dimensi. Fungsi move dioverride untuk mendukung pergerakan 3D, serta memiliki overload move yang menerima posisi target secara langsung. Fungsi status pada AerialDrone juga dioverride agar dapat menampilkan informasi ketinggian (altitude), posisi, dan kondisi baterai secara lebih spesifik. 

Kelas Amphibious merupakan gabungan dari GroundRobot dan AerialDrone. Kelas ini memiliki kemampuan untuk bergerak baik di darat maupun di udara/air dengan menggunakan pendekatan komposisi atau multiple inheritance. Dengan demikian, Amphibious dapat beralih mode sesuai kebutuhan misi dan kondisi lingkungan. 

Seluruh kendaraan yang ada akan dikelola menggunakan sebuah vektor berisi smart pointer unik, yaitu std::vector<std::unique_ptr<Vehicle>> fleet. Melalui vektor ini, setiap kendaraan dapat diakses secara polimorfik menggunakan pointer ke base class. Perintah pergerakan kendaraan dijalankan melalui fungsi executeMove yang akan memanggil versi fungsi move yang sesuai dengan jenis kendaraan. Dalam beberapa kasus, dynamic_cast digunakan untuk memastikan tipe kendaraan yang sedang digunakan, misalnya untuk membedakan antara drone dan robot darat. 

Proyek ini juga mensimulasikan sensor sederhana untuk mendeteksi rintangan (collision handling). GroundRobot memiliki implementasi khusus untuk fungsi obstacleAhead yang akan mengembalikan nilai true jika ada rintangan di depan dalam jarak tertentu. Jika rintangan terdeteksi, kendaraan akan memperpendek langkah atau menolak perintah gerak. Selain itu, setiap kendaraan juga memiliki sistem simulasi konsumsi baterai, di mana kapasitas baterai akan berkurang sesuai jarak tempuh. Konsep overloading dan overriding juga diterapkan secara eksplisit pada beberapa metode seperti move dan status untuk menunjukkan penerapan prinsip polymorphism di C++.

Proyek ini dikerjakan dalam bentuk kelompok dengan pembagian peran sebagai berikut. Anggota pertama bertugas membuat desain dasar dan kelas Vehicle, termasuk struct Position dan metode clone(). Anggota kedua mengimplementasikan kelas GroundRobot serta sistem obstacleAhead untuk mendeteksi rintangan. Anggota ketiga mengembangkan kelas AerialDrone beserta fungsi status yang menampilkan informasi lengkap serta move yang dioverload untuk bergerak ke posisi tertentu. Anggota keempat bertanggung jawab atas kelas Amphibious yang mengintegrasikan kemampuan darat dan udara sekaligus mengatur manajemen armada menggunakan vector dan fungsi executeMove. Anggota kelima bertugas membuat pengujian, skenario misi (scenarios.txt), dokumentasi, serta README dan demo run proyek.

Cara untuk menjalankan proyek ini cukup sederhana. Pertama, lakukan proses cloning repository menggunakan perintah: 
git clone https://github.com/username/vehicle-simulation.git 
dan masuk ke folder hasil clone dengan perintah cd vehicle-simulation. 

Untuk melakukan kompilasi program, gunakan perintah berikut sesuai sistem operasi yang digunakan. 

Pada Windows, gunakan:
g++ -std=c++17 -O2 -o main.exe *.cpp

Pada Linux, gunakan:
g++ -std=c++17 -O2 -o main *.cpp

Pada macOS, gunakan:
clang++ -std=c++17 -O2 -o main *.cpp

Setelah proses kompilasi selesai, jalankan program sesuai sistem operasi. Untuk Windows, jalankan main.exe, sedangkan pada Linux dan macOS jalankan ./main di terminal. 

Proyek ini juga menyediakan run script sederhana untuk menjalankan berbagai skenario. Berikut contoh perintah yang dapat disimpan dalam file scenarios.txt. 

MOVE G1 10 0 0  
MOVE G1 HEADING 90 DIST 5  
STATUS G1  
MOVE D1 5 5 10  
MOVE D1 TO 10 10 15  
STATUS D1  
MOVE A1 5 0 0  
MODE SWITCH TO AIR  
MOVE A1 0 0 20  
STATUS A1  

Program akan membaca file scenarios.txt dan mengeksekusi setiap perintah secara berurutan. Saat dijalankan, sistem akan menampilkan log status setiap kendaraan, misalnya: 

[INFO] GroundRobot G1 moved to (10, 0, 0)  
[WARNING] Obstacle detected ahead! Adjusting path...  
[INFO] AerialDrone D1 altitude: 15m, Battery: 83%  
[INFO] Amphibious A1 switched to air mode  
[INFO] Mission completed successfully!  

Struktur folder dalam proyek ini terdiri dari beberapa berkas sumber utama, yaitu vehicle.h/cpp, ground_robot.h/cpp, aerial_drone.h/cpp, amphibious.h/cpp, dan main.cpp, serta file scenarios.txt dan README.md atau README.txt untuk dokumentasi. 

Proyek ini dibuat untuk tujuan pembelajaran konsep dasar OOP, pewarisan (inheritance), overloading, overriding, serta polymorphism dalam bahasa C++. Seluruh kode dapat dijalankan di berbagai platform seperti Windows, Linux, maupun macOS menggunakan compiler C++17. File ini juga berfungsi sebagai panduan kompilasi, dokumentasi pembagian tugas, dan contoh cara menjalankan simulasi kendaraan menggunakan perintah berbasis teks.
