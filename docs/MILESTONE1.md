# Milestone 1

## Cara Kerja Interrupt
## Cara Kerja kernel.asm
Berkas kernel.asm memiliki 4 fungsi. Tiga di antaranya adalah fungsi antara
untuk membantu program yang dibentuk dari kode sumber pada kernel.c
berkomunikasi langsung dengan _hardware_ dan BIOS. Fungsi-fungsi ini akan
disebut sebagai fungsi pembantu.

Fungsi pembantu yang pertama adalah fungsi `putInMemory` yang memiliki 3
parameter, `segment` (int 2 byte), `address` (int 2 byte), dan `character`
(int 1 byte). Fungsi ini berfungsi "menaruh" `character` di memori pada
`segment` dan `offset` tertentu.  Penggunaan segmen dan _offset_ ini karena
cara _addressing_ memori pada prosesor yang diemulasikan pada tugas ini, Intel
8086, memiliki rumus: `segment`
<img src="https://render.githubusercontent.com/render/math?math=\times%202^4%20%2B"/>
`offset` agar dapat memiliki _address space_ sampai 1MB (selebihnya bisa lihat
[di sini](https://www.sciencedirect.com/topics/engineering/address-offset)).
Misalkan ingin diletakkan karakter 'A' di segmen video (0xB000) dengan _offset_
0x0A, maka penggunaan fungsinya adalah `putInMemory(0xB000, 0x0A, 'A')`.

Selanjutnya adalah fungsi `interrupt` dengan parameter: `number` (int 2 byte),
`AX` (int 2 byte), `BX` (int 2 byte), `CX` (int 2 byte), dan `DX` (int 2 byte).
Fungsi `interrupt` adalah fungsi untuk membuat _interrupt_ dengan nomor
_interrupt_ tertentu. Parameter `AX` sampai `DX` adalah nama-nama _register_
pada prosesor Intel 8086 dengan ukuran setiap _register_ adalah 2 byte (16
bit). _Register_-_register_ ini bertindak sebagai argumen untuk _interrupt_
yang dipanggil. Terakhir, parameter `number` adalah nomor _interrupt_ yang
ingin dipanggil. Misalkan, ingin dituliskan karakter 'A' berwarna biru ke layar
pada _page 0_, maka digunakan _interrupt_ 0x10 dengan isi _register_:
_Register_ | isi
-----------|----
`AX` | 0x0E00 + 'A'
`BX` | 0x0000 + 0x1
`CX` | 0
`DX` | 0
Untuk melihat isi _register_ yang harus digunakan untuk _interrupt_ tertentu,
dapat dilihat [di
sini](http://www.oldlinux.org/Linux.old/docs/interrupts/int-html/int.htm).

Fungsi bantuan terakhir pada kernel.asm adalah fungsi `makeInterrupt21`. Fungsi
ini akan digunakan untuk menggunakan _syscall_ pada OS yang akan dibuat nanti.
Fungsi ini untuk menyiapkan vektor/tabel _interrupt_ dengan nomor _interrupt_
0x21.

Terakhir, pada kernel.asm terdapat sebuah fungsi yang digunakan pada fungsi
`makeInterrupt21`, yaitu fungsi `interrupt21ServiceRoutine`. Fungsi ini
digunakan untuk menyimpan nilai-nilai pada `register` prosesor ke _stack_
sebelum melakukan operasi pada _interrupt_ 21 lalu mengembalikannya kembali
setelah _interrupt_ selesai dioperasikan.
