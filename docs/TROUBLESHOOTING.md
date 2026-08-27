# Troubleshooting Guide

## Common Issues

### "Cannot connect to x64dbg"

**Symptoms:** MCP server returns connection refused errors.

**Solutions:**
1. Ensure x64dbg plugin is loaded (check x64dbg log)
2. Verify REST API is running: `curl http://localhost:31964/health`
3. Check firewall isn't blocking port 31964

### "Unknown tool" errors

**Symptoms:** Claude reports tool not found.

**Solutions:**
1. Restart Claude Desktop after installation
2. Verify `claude_desktop_config.json` has correct path
3. Check server logs: `npm run dev` in `server/` directory

### Plugin not loading in x64dbg

**Symptoms:** Plugin doesn't appear in x64dbg plugin menu.

**Solutions:**
1. Ensure `.dp64` file is in `x64dbg/plugins/`
2. Check x64dbg version matches plugin architecture
3. Look for errors in x64dbg log (Help > About > Log)

### Build fails on Windows

**Symptoms:** CMake or npm errors during build.

**Solutions:**
```bash
# Clean and rebuild
rm -rf node_modules server/dist plugin/build
npm install
npm run build

# For plugin:
cd plugin
rm -rf build
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
```

### SSE connection drops

**Symptoms:** Events stop streaming after some time.

**Solutions:**
1. Increase keep-alive timeout in server config
2. Use stdio transport instead of HTTP+SSE
3. Check for network interruptions

## Debug Mode

Enable verbose logging:

```bash
# Server
export DEBUG=z-x64dbg:*
npm run dev

# Plugin
# Set registry: HKCU\Software\x64dbg\Debug = 1
```

## Getting Help

1. Check [GitHub Issues](https://github.com/Zekiog/z-x64dbg-mcp/issues)
2. Review logs in `server/` and x64dbg
3. Test REST API directly: `curl http://localhost:31964/health`

## Performance Tips

- Use hardware breakpoints when possible (faster)
- Batch memory reads/writes
- Limit trace count to avoid flooding
- Use SSE for real-time events, polling for state
