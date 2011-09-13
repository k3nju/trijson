#pragma once

namespace trijson
	{
	//-----------------------------------------------------------------------------------------//
	struct InputRange
		{
			inline InputRange( const char *h, const char *f )
				:begin( h ), end( f ), cur( h ), lineCount( 0 ){};
			inline bool IsValid(){ return cur < end; };
			inline bool SkipWhiteSpace()
				{
				while( cur < end &&
					   ( *cur == '\x20' ||
						 *cur == '\t'   ||
						 *cur == '\r'   ||
						 *cur == '\xb'  ||
						 *cur == '\xc'  ||
						 ( *cur == '\n' && ++lineCount ) ) )
					++cur;

				return IsValid();
				}
			inline size_t GetRemainingSize(){ return end - cur; };
			inline size_t GetConsumedSize(){ return cur - begin; };
			inline void Forward( size_t s ){ cur +=s; };

			const char *begin;
			const char *end;
			const char *cur;
			size_t lineCount;
		};
	}
