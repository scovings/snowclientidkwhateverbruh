@echo off
setlocal


git pull
:: Ask for a commit message
set /p commitMessage=Enter commit message: 

:: Add all changes
git add .

:: Commit with user-provided message
git commit -m "%commitMessage%"

:: Push to the main branch
git push origin main

endlocal
