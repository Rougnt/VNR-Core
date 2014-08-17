#pragma once

// engine/engine_p.h
// 8/23/2013 jichi
// See: http://ja.wikipedia.org/wiki/プロジェクト:美少女ゲーム系/ゲームエンジン

#include "config.h"

struct HookParam; // defined in ith types.h

namespace Engine {

// Global variables
extern wchar_t process_name_[MAX_PATH]; // cached
extern DWORD module_base_,
             module_limit_;

//extern LPVOID trigger_addr;
typedef bool (* trigger_fun_t)(LPVOID addr, DWORD frame, DWORD stack);
extern trigger_fun_t trigger_fun_;

// Wii engines

bool InsertGCHooks(); // Dolphin
bool InsertVanillawareGCHook();

// PS2 engines

bool InsertPCSX2Hooks(); // PCSX2
bool InsertMarvelousPS2Hook();  // http://marvelous.jp
bool InsertMarvelous2PS2Hook(); // http://marvelous.jp
bool InsertTypeMoonPS2Hook();   // http://typemoon.com
//bool InsertNamcoPS2Hook();

// PSP engines

void SpecialPSPHook(DWORD esp_base, HookParam *hp, DWORD *data, DWORD *split, DWORD *len); // General PSP extern hook

bool InsertPPSSPPHooks();        // PPSSPPWindows

bool InsertPPSSPPHLEHooks();

bool InsertPPSSPP099Hooks();
bool InsertOtomatePPSSPPHook(); // PSP otomate.jp, 0.9.9 only

bool Insert5pbPSPHook();        // PSP 5pb.jp
bool InsertAlchemistPSPHook();  // PSP Alchemist-net.co.jp, 0.9.8 only
bool InsertAlchemist2PSPHook(); // PSP Alchemist-net.co.jp
bool InsertBandaiNamePSPHook(); // PSP Bandai.co.jp
bool InsertBandaiPSPHook();     // PSP Bandai.co.jp
bool InsertBroccoliPSPHook();   // PSP Broccoli.co.jp

bool InsertCyberfrontPSPHook(); // PSP CYBERFRONT (closed)
bool InsertImageepochPSPHook(); // PSP Imageepoch.co.jp
bool InsertImageepoch2PSPHook();// PSP Imageepoch.co.jp
bool InsertKadokawaNamePSPHook(); // PSP Kadokawa.co.jp
bool InsertKonamiPSPHook();     // PSP Konami.jp
bool InsertTecmoPSPHook();      // PSP Koeitecmo.co.jp
//bool InsertTypeMoonPSPHook(); // PSP Typemoon.com

bool InsertOtomatePSPHook();    // PSP Otomate.jp, 0.9.8 only

bool InsertIntensePSPHook();    // PSP Intense.jp
bool InsertKidPSPHook();        // PSP Kid-game.co.jp
bool InsertNippon1PSPHook();    // PSP Nippon1.jp
bool InsertNippon2PSPHook();    // PSP Nippon1.jp
bool InsertYetiPSPHook();       // PSP Yetigame.jp
bool InsertYeti2PSPHook();      // PSP Yetigame.jp

// PC engines

bool Insert2RMHook();           // 2RM - Adventure Engine
bool InsertAB2TryHook();        // Yane@AkabeiSoft2Try: YaneSDK.dll.
bool InsertAbelHook();          // Abel
bool InsertAdobeAirHook();      // Adobe AIR
bool InsertAliceHook();         // System40@AliceSoft; do not work for latest alice games
bool InsertAmuseCraftHook();    // AMUSE CRAFT: *.pac
bool InsertAnex86Hook();        // Anex86: anex86.exe
bool InsertAOSHook();           // AOS: *.aos
bool InsertApricotHook();       // Apricot: arc.a*
bool InsertArtemisHook();       // Artemis Engine: *.pfs
bool InsertAtelierHook();       // Atelier Kaguya: message.dat
bool InsertBGIHook();           // BGI: BGI.*
bool InsertC4Hook();            // C4: C4.EXE or XEX.EXE
bool InsertCaramelBoxHook();    // Caramel: *.bin
bool InsertCandyHook();         // SystemC@CandySoft: *.fpk
bool InsertCatSystem2Hook();    // CatSystem2: *.int
bool InsertCMVSHook();          // CMVS: data/pack/*.cpz; do not support the latest cmvs32.exe and cmvs64.exe
bool InsertCotophaHook();       // Cotopha: *.noa
bool InsertDebonosuHook();      // Debonosu: bmp.bak and dsetup.dll
bool InsertEaglsHook();         // E.A.G.L.S: EAGLES.dll
bool InsertEMEHook();           // EmonEngine: emecfg.ecf
bool InsertEushullyHook();      // Eushully: AGERC.DLL
bool InsertGesen18Hook();       // Gsen18: *.szs
bool InsertGXPHook();           // GXP: *.gxp
bool InsertLiveHook();          // Live: live.dll
bool InsertMalieHook();         // Malie@light: malie.ini
bool InsertMajiroHook();        // Majiro: *.arc
bool InsertMarineHeartHook();   // Marine Heart: SAISYS.exe
bool InsertMBLHook();           // MBL: *.mbl
bool InsertMEDHook();           // MED: *.med
bool InsertMonoHook();          // Mono (Unity3D): */Mono/mono.dll
bool InsertNeXASHook();         // NeXAS: Thumbnail.pac
bool InsertNextonHook();        // NEXTON: aInfo.db
bool InsertNexton1Hook();
bool InsertNitroPlusHook();     // NitroPlus: *.npa
bool InsertPensilHook();        // Pensil: PSetup.exe
bool InsertQLIEHook();          // QLiE: GameData/*.pack
//bool InsertRai7Hook();          // Rai7puk: rai7.exe
bool InsertRejetHook();         // Rejet: Module/{gd.dat,pf.dat,sd.dat}
bool InsertRUGPHook();          // rUGP: rUGP.exe
bool InsertRetouchHook();       // Retouch: resident.dll
bool InsertRREHook();           // RunrunEngine: rrecfg.rcf
bool InsertShinaHook();         // ShinaRio: Rio.ini
bool InsertShinyDaysHook();     // ShinyDays
bool InsertElfHook();           // elf: Silky.exe
bool InsertSystem43Hook();      // System43@AliceSoft: AliceStart.ini
bool InsertSiglusHook();        // SiglusEngine: SiglusEngine.exe
bool InsertSideBHook();         // SideB: Copyright side-B
bool InsertTanukiHook();        // Tanuki: *.tak
bool InsertTaskforce2Hook();    // Taskforce2.exe
bool InsertTencoHook();         // Tenco: Check.mdx
bool InsertTriangleHook();      // Triangle: Execle.exe
bool InsertScenarioPlayerHook();// sol-fa-soft: *.iar && *.sec5
bool InsertYukaSystem2Hook();   // YukaSystem2: *.ykc
bool InsertWhirlpoolHook();     // YU-RIS: *.ypf
bool InsertWillPlusHook();      // WillPlus: Rio.arc
bool InsertWolfHook();          // Wolf: Data.wolf

void InsertBrunsHook();         // Bruns: bruns.exe
void InsertKiriKiriHook();      // KiriKiri: *.xp3, resource string
void InsertIronGameSystemHook();// IroneGameSystem: igs_sample.exe
void InsertLucifenHook();       // Lucifen@Navel: *.lpk
void InsertRyokuchaHook();      // Ryokucha: _checksum.exe
void InsertRealliveHook();      // RealLive: RealLive*.exe
void InsertSoftHouseHook();     // SoftHouse: *.vfs
void InsertStuffScriptHook();   // Stuff: *.mpk
void InsertTinkerBellHook();    // TinkerBell: arc00.dat
void InsertWaffleHook();        // WAFFLE: cg.pak

// CIRCUS: avdata/
bool InsertCircusHook1();
bool InsertCircusHook2();

} // namespace Engine

// EOF
