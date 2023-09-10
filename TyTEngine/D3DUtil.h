#pragma once

#ifndef D3DUTIL_H
#define D3DUTIL_H

#define ReleaseCOM(x) { if(x){x->Release(); x= 0; }}

#endif