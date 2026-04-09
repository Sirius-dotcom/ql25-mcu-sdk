param(
    [Parameter(Mandatory = $true)]
    [string]$InputFile,

    [Parameter(Mandatory = $true)]
    [string]$OutputFile
)

$words = foreach ($line in Get-Content -Path $InputFile) {
    $trimmed = $line.Trim()
    if (-not $trimmed) {
        continue
    }
    if ($trimmed.StartsWith("@")) {
        continue
    }

    foreach ($word in ($trimmed -split "\s+")) {
        if ($word) {
            $word.ToUpperInvariant()
        }
    }
}

Set-Content -Path $OutputFile -Value $words -Encoding ascii
