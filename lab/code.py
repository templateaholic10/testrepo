# coding: UTF-8
"""
source alphabet \mathcal{x} = {a,b,c,d}
code alphabet \mathcal{y} = {0,1}
"""

def huffman(source_alphabet, code_alphabet, pmf):
    """
    make 1-symbol binary huffman code, return <f,g>.
    """
    # 部分木の根ノードを保持する．
    # 各ノードは[生起確率, 対応する葉たちのリスト]という構成になっている．
    roots = [(p,(s_a,)) for (s_a,p) in zip(source_alphabet,pmf)]

    # 符号器を初期化
    f = {s_a: "" for s_a in source_alphabet}

    # 符号木が連結になるまで
    while len(roots) > 1:
        new_pr = 0.
        new_leaves = []
        # 符号アルファベットの数だけ部分木をまとめる．
        for c_a in code_alphabet:
            if len(roots) == 0:
                break
            node = min(roots, key=(lambda node: node[0]))
            new_pr += node[0]
            new_leaves += node[1]
            for s_a in node[1]:
                f[s_a] = c_a + f[s_a]
            roots.remove(node)

        # まとめた部分木の根ノードをrootsに追加．
        roots.append((new_pr, tuple(new_leaves)))

    g = {value: key for key,value in f.items()}
    return f,g

def tunstall(source_alphabet, code_alphabet, pmf):
    """
    make 1-symbol binary tunstall code, return <f,g>.
    """
    # 符号木の葉ノードを保持する．
    # 各ノードは[生起確率, 対応する情報源系列]という構成になっている．
    leaves = [(1., "")]

    # すべての符号アルファベットが葉ノードに割り当てられるようになるまで
    # ぴったりにならなければ符号アルファベットを余らせる．
    while len(leaves) <= len(code_alphabet) - len(source_alphabet):
        branch = max(leaves, key=(lambda node: node[0]))
        leaves.remove(branch)
        # 情報源アルファベットの数だけノードを分枝する．
        for s_a,pr in zip(source_alphabet,pmf):
            leaves.append((branch[0]*pr, branch[1]+s_a))

    # 最終的な葉ノードに符号語を割り当てる．
    f = {leaf[1]: c_a for leaf,c_a in zip(leaves, code_alphabet)}
    g = {value: key for key,value in f.items()}
    return f,g

def encode(source_sequence, encodedict):
    """
    encode source sequence with dictionary, return code sequence.
    """
    code_sequence = ""
    source_word = ""
    for a in source_sequence:
        source_word += a
        if source_word in encodedict.keys():
            code_sequence += encodedict[source_word]
            source_word = ""
    return code_sequence

def decode(code_sequence, decodedict):
    """
    decode code sequence with dictionary, return source sequence.
    """
    source_sequence = ""
    code_word = ""
    for a in code_sequence:
        code_word += a
        if code_word in decodedict.keys():
            source_sequence += decodedict[code_word]
            code_word = ""
    return source_sequence

if __name__ == "__main__":
    # ハフマン符号の作成
    source_alphabet = ("a", "b", "c", "d")
    code_alphabet = ("0", "1", "2")
    pmf = (0.4, 0.3, 0.2, 0.1)
    f,g = huffman(source_alphabet=source_alphabet, code_alphabet=code_alphabet, pmf=pmf)
    print "huffman table: %s" % f

    source_sequence = "abcd"
    # ハフマン符号化
    code_sequence = encode(source_sequence=source_sequence, encodedict=f)
    print "(source)%s -> (code)%s" % (source_sequence, code_sequence)
    # ハフマン復号化
    source_sequence = decode(code_sequence=code_sequence, decodedict=g)
    print "(code)%s -> (source)%s" % (code_sequence, source_sequence)

    # タンストール符号の作成
    source_alphabet = ("a", "b")
    code_alphabet = ("000", "001", "010", "011", "100", "101", "110", "111")
    pmf = (0.7, 0.3)
    f,g = tunstall(source_alphabet=source_alphabet, code_alphabet=code_alphabet, pmf=pmf)
    print "tunstall table: %s" % f

    source_sequence = "abaaababababbbbaababababbaaababbbabaa"
    # タンストール符号化
    code_sequence = encode(source_sequence=source_sequence, encodedict=f)
    print "(source)%s -> (code)%s" % (source_sequence, code_sequence)
    # タンストール復号化
    source_sequence = decode(code_sequence=code_sequence, decodedict=g)
    print "(code)%s -> (source)%s" % (code_sequence, source_sequence)
