; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppVersion "0.10.0"
#define MyAppPublisher "Rok Ajdnik"
#define MyAppExeName "WorkSchedule.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{52E34DD8-AAD7-4EE7-836D-8FF9CFDA19CB}
AppName={cm:MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={userpf}\{cm:MyAppName}
DefaultGroupName={cm:MyAppName}
OutputBaseFilename=setup
SetupIconFile={#SourcePath}\src\app.ico
Compression=lzma
SolidCompression=yes
PrivilegesRequired=lowest

[Languages]
Name: en; MessagesFile: "compiler:Default.isl"
Name: "sl"; MessagesFile: "compiler:Languages\Slovenian.isl"

[CustomMessages]
en.MyAppName=Work schedule
sl.MyAppName=Urnik dela

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#SourcePath}\src\appdata_sl.db"; DestDir: "{userappdata}\WorkSchedule"; DestName: "appdata.db"; Flags: ignoreversion uninsneveruninstall onlyifdoesntexist; Languages: sl
Source: "{#SourcePath}\src\appdata_en.db"; DestDir: "{userappdata}\WorkSchedule"; DestName: "appdata.db"; Flags: ignoreversion uninsneveruninstall onlyifdoesntexist; Languages: en
Source: "{#SourcePath}\publish\*.*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{cm:MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{cm:MyAppName}}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\{cm:MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon


