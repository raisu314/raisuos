# RaisuOS All Device Native Work System (ADNWS) 

RaisuOS is a completely original, independent operating system built for the x86_64 (x64) architecture. It is built natively with zero reliance on Linux, Windows Subsystem for Linux (WSL), or any existing kernel base from other systems.

## 🌟 The ADNWS Architecture
RaisuOS pioneers the **All Device Native Work System**. 
This means that whether you boot RaisuOS on a high-end desktop PC or a compatible smartphone/tablet form factor, the system actively scales its processes to provide the identical robust operating environment everywhere.

**System Requirements (Hard Enforced):**
- **Architecture**: x86_64 (64-bit Long Mode)
- **Minimum RAM**: 4 GB (Strict). Supports up to **1 TB**.
- **Minimum Storage (ROM)**: 32 GB. Supports up to **8 TB** (Main) / Unlimited (Expansion).
- **Display**: VBE Compatible Framebuffer (1024x768 minimum)

## ☁️ Zero-Dependency Local Build System
You **do not** need to install any toolchains, Linux, or WSL on your computer. RaisuOS compiles completely error-free in the cloud.

1. Fork or push to your repository.
2. **GitHub Actions** immediately processes the `.github/workflows/build.yml`.
3. The isolated cloud environment automatically targets `x86_64-elf`, links the kernel, generates the Multiboot2 GRUB ISO, and uploads the artifacts.
4. Download the `raisuos.iso` from the Actions tab.

## ⚙️ Features
- **Seamless 64-bit Core**: Full CPU protection rings, 4-level paging, IDT/GDT structures rebuilt for Long Mode.
- **Graphic Interface (RaisuDE)**: Double-buffered VBE compositing window manager written completely from scratch.
- **Multilingual Support**: Realtime language switching (EN, JA, FI).

---
*RaisuOS - 100% Original. Zero Linux. Zero Compromises.*
