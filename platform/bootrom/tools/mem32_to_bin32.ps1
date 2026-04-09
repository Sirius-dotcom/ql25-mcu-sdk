param(
    [Parameter(Mandatory = $true)]
    [string]$InputFile,

    [Parameter(Mandatory = $true)]
    [string]$OutputFile
)

$lines = foreach ($line in Get-Content -Path $InputFile) {
    $trimmed = $line.Trim()
    if (-not $trimmed) {
        continue
    }

    $value = [Convert]::ToUInt32($trimmed, 16)
    [Convert]::ToString($value, 2).PadLeft(32, '0')
}

Set-Content -Path $OutputFile -Value $lines -Encoding ascii
