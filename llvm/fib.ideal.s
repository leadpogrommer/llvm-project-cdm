asect 0
dc main, 0
main:
jsr test
halt

test:                                   # -- Begin function test
# %bb.0:                                # %entry
            # sp -> return address
    push fp # sp -> saved fp
    ldsp fp # fp -> saved fp

    addsp -10 # 2x local 16 bit variables

	ssw	r1, -2                          # 2-byte Folded Spill
    ldi	r0, 5
    ssw	r0, -4
    ldi	r0, 2
    ssw	r0, -6
    lsw	r0, -4
    lsw	r1, -6
    add r0, r1, r0
    ssw	r0, -8
    lsw	r0, -6
    ldi	r1, 1
    add r0, r1, r0
    ssw	r0, -10
    lsw	r0, -8
    lsw	r1, -10
    add r0, r1, r0
    lsw	r1, -2                          # 2-byte Folded Reload


	# ignore alloca
	addsp 10 # sp -> saved fp
	pop fp

	rts
                                        # -- End function
end.