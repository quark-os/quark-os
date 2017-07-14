.intel_syntax noprefix

#########################################################################
# 	Free Block Structure	Total Size:	20 bytes + Open Space			#
#########################################################################
#	Flag		Size		Link		Open		Flag		Size	#
# 	uint32_t	uint32_t	uint32_t	<undef>		uint32_t	uint32_t#
#########################################################################
#########################################################################
# 	Res. Block Structure	Total Size:	12 bytes + Open Space			#
#########################################################################
#	Flag		Size		Open		Flag							#
# 	uint32_t	uint32_t	<undef>		uint32_t						#
#########################################################################

.global initializeHeap
initializeHeap:				# initializeHeap(void* heapLocation, uint32_t size)
	mov ecx, [esp + 8]		# Load size of heap in bytes
	mov edx, [esp + 4]		# Load location of heap
	sub ecx, 20
	movw [edx], 0x9999
	movw [edx + 2], 0x9999	# Write 'free' flag at start of heap
	mov [edx + 4], ecx		# Write size of heap
	movw [edx + 8], 0		
	movw [edx + 10], 0		# Write link to next block (no next block; null pointer)
	movw [edx + ecx + 12], 0x9999	
	movw [edx + ecx + 14], 0x9999	# Write 'free' flag at end of heap
	mov [edx + ecx + 16], ecx		# Write size of heap
	ret

.global allocate
allocate:					# allocate(FreeBlockHeader* avail, uint32_t size);
	mov ecx, [esp + 8]		# Load number of bytes to reserve
	mov edx, [esp + 4]		# Load pointer to structure linking to first element in free space table
	push ebx
	push esi
	mov ebx, edx
checkblock:
	mov eax, [ebx + 8]		# Load LINK(ebx) where ebx points to a free block
	or eax, eax
	jz err					# If pointer is null, return w/ error (out of memory!)
	mov edx, [eax + 4]		# Load size of empty space in block
	add edx, 8				# Add size of header information that can be freed
	cmp ecx, edx
	jle reserve				# Test if block is big enough. If so, reserve memory.
	mov ebx, eax			# Else, shift to next block in list.
	jmp checkblock
reserve:
	add ecx, 12				# Account for 12 byte overhead to reserve a block
	cmp ecx, [eax + 4]		# Check if open spce in block is enough.
	jg remove_block			# If not, the whole block must be reserved.
	sub edx, 8				# Block will be preserved; cannot free header information
	sub edx, ecx			# Subtract space to be allocated from size of block	
	mov esi, eax			# Copy pointer to free block
	add esi, 12				# Add size of header information
	add esi, edx			# Add size of remaining free space
	movw [esi], 0x9999		
	movw [esi + 2], 0x9999	# Write 'free' flag at new end of block
	mov [esi + 4], edx		# Write new size
	sub ecx, 12				# Subtract size of header information from size of new reserved block
	movw [esi + 8], 0xFFFF
	movw [esi + 10], 0xFFFF	# Write 'reserved' flag at beginning of reserved block
	mov [esi + 12], ecx		# Write 'size' tag at beginning of reserved block
	movw [esi + ecx + 12], 0xFFFF
	movw [esi + ecx + 14], 0xFFFF	# Write 'reserved' flag at end of reserved block
	mov eax, esi			# Save location of free block footer to return register
	add eax, 16				# Add size of free block footer and reserved block header; pointer now leads to reserved space
	jmp exit				# Exit function, returning pointer to allocated space
remove_block:
	sub ecx, 12				# Overhead will just overwrite existing block headers; does not need to be accounted for
	mov edx, [eax + 8]		# Load link from block to be removed
	mov [ebx + 8], edx		# Place that link in the previous block, cutting the current one from the table
	movw [eax], 0xFFFF		# Set block flag to reserved
	movw [eax + 2], 0xFFFF
	mov edx, [eax + 4]
	add edx, 8
	add [eax + 4], edx		# Adjust 'size' tag to account for freed header information
	mov esi, eax			
	add esi, 8
	add esi, [eax + 4]		# Calculate position of end flag
	movw [esi], 0xFFFF		# Set block flag to reserved
	movw [esi + 2], 0xFFFF
	add eax, 8				# Shift EAX to point to the allocated memory
	jmp exit			# Exit function, returning pointer to allocated space
err:
	xor eax, eax			# Set EAX to 0 (null pointer)
							# Exit function, return null pointer
exit:
	pop esi
	pop ebx					# Pop registers the function is required to preserve
	ret						# Return
	
