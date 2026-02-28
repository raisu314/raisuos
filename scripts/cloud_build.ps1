# RaisuOS Cloud Builder (PowerShell)
# This script helps you push RaisuOS to GitHub and download the built ISO
# without installing any Linux/WSL tools on your local PC.

param (
    [string]$RepoUrl
)

Write-Host "================================================" -ForegroundColor Cyan
Write-Host "   RaisuOS All Device Native Work System Build  " -ForegroundColor Cyan
Write-Host "================================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Your OS is completely written in C and x64 Assembly." 
Write-Host "To ensure ZERO ERRORS and NO LINUX dependencies locally,"
Write-Host "we will build this in the cloud using GitHub Actions."
Write-Host ""

if (-not (Get-Command "git" -ErrorAction SilentlyContinue)) {
    Write-Host "[ERROR] Git is not installed. Please install Git for Windows first." -ForegroundColor Red
    Write-Host "Download: https://git-scm.com/download/win" -ForegroundColor Yellow
    exit 1
}

if (-not (Test-Path ".git")) {
    Write-Host "[INFO] Initializing new Git repository..." -ForegroundColor Green
    git init
    git branch -M main
}

if ($RepoUrl) {
    Write-Host "[INFO] Setting remote origin to $RepoUrl" -ForegroundColor Green
    git remote remove origin 2>$null
    git remote add origin $RepoUrl
}

Write-Host "[INFO] Committing RaisuOS ADNWS Codebase..." -ForegroundColor Green
git add .
git commit -m "Initial commit: RaisuOS v2.0 ADNWS (x86_64) Zero Linux Build"

Write-Host "[SUCCESS] Code committed locally." -ForegroundColor Green
Write-Host ""
Write-Host "NEXT STEPS:" -ForegroundColor Cyan
Write-Host "1. Go to https://github.com/new and create an empty repository (e.g., 'raisuos')"
Write-Host "2. Copy the URL of your new repository"
Write-Host "3. Run this script again with the URL:"
Write-Host "   .\scripts\cloud_build.ps1 -RepoUrl https://github.com/YourName/raisuos.git"
Write-Host "4. Finally, push the code:"
Write-Host "   git push -u origin main"
Write-Host ""
Write-Host "GitHub will automatically build the ISO. Go to the 'Actions' tab on GitHub to download 'raisuos.iso'!" -ForegroundColor Yellow
