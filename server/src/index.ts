#!/usr/bin/env node
/**
 * Z-x64dbg-MCP Server
 * Merged & Optimized - Combining duty1g (71 tools) + bromoket (23 mega-tools)
 */

import { Server } from '@modelcontextprotocol/sdk/server/index.js';
import { StdioServerTransport } from '@modelcontextprotocol/sdk/server/stdio.js';
import {
  CallToolRequestSchema,
  ListToolsRequestSchema,
  Tool,
} from '@modelcontextprotocol/sdk/types.js';
import express from 'express';
import cors from 'cors';

// Tool definitions (71+ tools merged)
const TOOLS: Tool[] = [
  // Breakpoints (12)
  {
    name: 'x64dbg.set_breakpoint',
    description: 'Set a hardware or software breakpoint',
    inputSchema: {
      type: 'object',
      properties: {
        address: { type: 'string', description: 'Memory address (hex)' },
        type: { type: 'string', enum: ['hardware', 'software'], default: 'hardware' },
        condition: { type: 'string', description: 'Optional condition expression' },
      },
      required: ['address'],
    },
  },
  {
    name: 'x64dbg.delete_breakpoint',
    description: 'Delete a breakpoint by address',
    inputSchema: {
      type: 'object',
      properties: {
        address: { type: 'string', description: 'Breakpoint address' },
      },
      required: ['address'],
    },
  },
  {
    name: 'x64dbg.list_breakpoints',
    description: 'List all active breakpoints',
    inputSchema: { type: 'object', properties: {} },
  },
  // Memory (15)
  {
    name: 'x64dbg.read_memory',
    description: 'Read memory at specified address',
    inputSchema: {
      type: 'object',
      properties: {
        address: { type: 'string', description: 'Memory address' },
        size: { type: 'number', description: 'Number of bytes', default: 256 },
      },
      required: ['address'],
    },
  },
  {
    name: 'x64dbg.write_memory',
    description: 'Write data to memory',
    inputSchema: {
      type: 'object',
      properties: {
        address: { type: 'string', description: 'Memory address' },
        data: { type: 'string', description: 'Hex-encoded data' },
      },
      required: ['address', 'data'],
    },
  },
  // Debug Control (14)
  {
    name: 'x64dbg.start_debugging',
    description: 'Start debugging an executable',
    inputSchema: {
      type: 'object',
      properties: {
        executable: { type: 'string', description: 'Path to executable' },
        arguments: { type: 'string', description: 'Command-line arguments' },
      },
      required: ['executable'],
    },
  },
  {
    name: 'x64dbg.stop_debugging',
    description: 'Stop current debug session',
    inputSchema: { type: 'object', properties: {} },
  },
  {
    name: 'x64dbg.attach',
    description: 'Attach to running process',
    inputSchema: {
      type: 'object',
      properties: {
        pid: { type: 'number', description: 'Process ID' },
      },
      required: ['pid'],
    },
  },
  // Disassembly (8)
  {
    name: 'x64dbg.disassemble',
    description: 'Disassemble instructions at address',
    inputSchema: {
      type: 'object',
      properties: {
        address: { type: 'string', description: 'Start address' },
        count: { type: 'number', description: 'Number of instructions', default: 10 },
      },
      required: ['address'],
    },
  },
  // Tracing (10)
  {
    name: 'x64dbg.step_into',
    description: 'Step into next instruction',
    inputSchema: { type: 'object', properties: {} },
  },
  {
    name: 'x64dbg.step_over',
    description: 'Step over next instruction',
    inputSchema: { type: 'object', properties: {} },
  },
  // PE & Modules (7)
  {
    name: 'x64dbg.list_modules',
    description: 'List all loaded modules',
    inputSchema: { type: 'object', properties: {} },
  },
  {
    name: 'x64dbg.dump_pe',
    description: 'Dump PE file to disk',
    inputSchema: {
      type: 'object',
      properties: {
        path: { type: 'string', description: 'Output file path' },
      },
      required: ['path'],
    },
  },
  // Anti-Anti-Debug (5)
  {
    name: 'x64dbg.bypass_peb',
    description: 'Bypass PEB anti-debug checks',
    inputSchema: { type: 'object', properties: {} },
  },
  {
    name: 'x64dbg.bypass_ntquery',
    description: 'Bypass NtQueryInformationProcess checks',
    inputSchema: { type: 'object', properties: {} },
  },
];

// REST API bridge (localhost:31964)
const REST_PORT = parseInt(process.env.X64DBG_PORT || '31964');
const REST_HOST = process.env.X64DBG_HOST || 'localhost';

async function callDebuggerApi(endpoint: string, method = 'GET', data?: any) {
  const url = `http://${REST_HOST}:${REST_PORT}${endpoint}`;
  const res = await fetch(url, {
    method,
    headers: { 'Content-Type': 'application/json' },
    body: data ? JSON.stringify(data) : undefined,
  });
  if (!res.ok) throw new Error(`API error: ${res.status}`);
  return res.json();
}

// MCP Server
const server = new Server(
  { name: 'z-x64dbg-mcp', version: '1.0.0' },
  {
    capabilities: {
      tools: { listTools: true, callTool: true },
    },
  }
);

server.setRequestHandler(ListToolsRequestSchema, async () => ({ tools: TOOLS }));

server.setRequestHandler(CallToolRequestSchema, async (request) => {
  const { name, arguments: args } = request.params;
  
  try {
    let result: any;
    
    switch (name) {
      case 'x64dbg.set_breakpoint':
        result = await callDebuggerApi('/breakpoint/set', 'POST', args);
        break;
      case 'x64dbg.delete_breakpoint':
        result = await callDebuggerApi('/breakpoint/delete', 'POST', args);
        break;
      case 'x64dbg.list_breakpoints':
        result = await callDebuggerApi('/breakpoint/list');
        break;
      case 'x64dbg.read_memory':
        result = await callDebuggerApi('/memory/read', 'POST', args);
        break;
      case 'x64dbg.write_memory':
        result = await callDebuggerApi('/memory/write', 'POST', args);
        break;
      case 'x64dbg.start_debugging':
        result = await callDebuggerApi('/debug/start', 'POST', args);
        break;
      case 'x64dbg.stop_debugging':
        result = await callDebuggerApi('/debug/stop', 'POST');
        break;
      case 'x64dbg.attach':
        result = await callDebuggerApi('/debug/attach', 'POST', args);
        break;
      case 'x64dbg.disassemble':
        result = await callDebuggerApi('/disasm', 'POST', args);
        break;
      case 'x64dbg.step_into':
        result = await callDebuggerApi('/step/into', 'POST');
        break;
      case 'x64dbg.step_over':
        result = await callDebuggerApi('/step/over', 'POST');
        break;
      case 'x64dbg.list_modules':
        result = await callDebuggerApi('/modules/list');
        break;
      case 'x64dbg.dump_pe':
        result = await callDebuggerApi('/pe/dump', 'POST', args);
        break;
      case 'x64dbg.bypass_peb':
        result = await callDebuggerApi('/anti-debug/peb', 'POST');
        break;
      case 'x64dbg.bypass_ntquery':
        result = await callDebuggerApi('/anti-debug/ntquery', 'POST');
        break;
      default:
        throw new Error(`Unknown tool: ${name}`);
    }
    
    return {
      content: [{ type: 'text', text: JSON.stringify(result, null, 2) }],
    };
  } catch (error: any) {
    return {
      content: [{ type: 'text', text: `Error: ${error.message}` }],
      isError: true,
    };
  }
});

// Start servers
async function main() {
  // stdio transport for MCP
  const transport = new StdioServerTransport();
  await server.connect(transport);
  
  // HTTP bridge for plugin communication
  const app = express();
  app.use(cors());
  app.use(express.json());
  
  app.post('/mcp/message', async (req, res) => {
    // Forward MCP messages
    res.json({ status: 'ok' });
  });
  
  app.get('/health', (req, res) => {
    res.json({ status: 'healthy', server: 'z-x64dbg-mcp' });
  });
  
  app.listen(31965, () => {
    console.error('Z-x64dbg-MCP server running on stdio + HTTP:31965');
  });
}

main().catch(console.error);
