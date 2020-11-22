################################################################################################################
#                                         Usual fonctions for the cutting                                      #
################################################################################################################


# Cut the matrix according to a height i or a width j
# the cutting function preserve the reading direction because in occidental language,
# we read from top to bottom and from left to right as the cutting function work
def cut_matrix(M, i, j):
    L = []
    if i:
        # Allow to have two matrix with i lines and len(M)-i lines from one matrix
        M0 = []
        M1 = []
        for line in range(i):
            M0.append(M[line])
        for line in range(i, len(M)):
            M1.append(M[line])
        L.append(M0)
        L.append(M1)

    elif j:
        # Allow to have two matrix with j columns and len(M[0])-j column from one matrix
        M2 = []
        M3 = []
        for line in range(len(M)):
            M2.append([])
            for col in range(j):
                M2[line].append(M[line][col])
        for line in range(len(M)):
            M3.append([])
            for col in range(j, len(M[0])):
                M3[line].append(M[line][col])
        L.append(M2)
        L.append(M3)
    elif i == 0 or j == 0 or i == len(M) or j == len(M[0]):
        return M
    return L


# Create an empty histogram (initialized at 0)
def create_histogram(nb):
    H = []
    for i in range(nb):
        H.append(0)
    return H


# Using the threshold and the length of the matrix, return a list of row(or columns)
# where we will need to cut
def thresholding(H, s, l):
    # Allow to choose where we need to cut in function of the threshold
    n = 0
    L = []
    for i in range(len(H)):
        if H[i] == l:
            n += 1
        else:
            if n > s:
                L.append(i - n // 2)
            n = 0
    return L


# Remove white borders from the matrix
def wipe_white_borders(M):
    M = wipe_vertical(M)
    return wipe_horizontal(M)


# Remove vertical white borders from the matrix by using a histogram and searching white column at the begin or
# the ending of the matrix
def wipe_vertical(M):
    (l, l0) = (len(M), len(M[0]))
    HV = create_histogram(l0)
    for col in range(l0):
        for line in range(l):
            HV[col] += (M[line][col] // 255)

    i = 0
    l0max = l0
    # Search white column at the beginning
    while i < l0max and HV[i] == l:
        i += 1
        l0 -= 1
        for j in range(l):
            M[j].pop(0)

    if i == l0max:
        return None

    l0max -= 1
    # Search white column at the end
    while l0max > i and HV[l0max] == l:
        for j in range(l):
            M[j].pop()
        l0max -= 1

    return M


# Remove horizontal white borders from the matrix by using a histogram and searching white row at the begin or
# the ending of the matrix
def wipe_horizontal(M):
    (l, l0) = (len(M), len(M[0]))
    HH = create_histogram(l)
    for line in range(l):
        for col in range(l0):
            HH[line] += (M[line][col] // 255)
    i = 0
    lmax = l
    # Search white rows at the beginning
    while i < lmax and HH[i] == l0:
        i += 1
        l -= 1
        M.pop(0)
    if i == lmax:
        return None
    lmax -= 1
    # Search white rows at the beginning
    while lmax > i and HH[lmax] == l0:
        M.pop()
        lmax -= 1

    return M


# Transform the matrix into a square matrix by adding white pixels.
def square_reshaped(M):
    (l, l0) = (len(M), len(M[0]))
    if l > l0:
        for j in range(l0, l):
            for i in range(l):
                M[i].append(255)
    elif l < l0:
        for i in range(l, l0):
            M.append([])
            for j in range(l0):
                M[i].append(255)
    return M


# Resize a matrix in the conform size to be called by the neural network (16x16)
def adjust_matrix_size(M):
    square_reshaped(M)
    (l, l0) = (len(M), len(M[0]))
    if l == 16 and l0 == 16:
        return M
    adjusted_M = []
    # Calculate the factor of enlargement or narrowing of the matrix
    height_factor_ = (float)(l / 16)
    width_factor = (float)(l0 / 16)

    # Calculate the pixel of the 16x16 matrix with the old matrix and the factor.
    # The calculation depend on the size of the matrix (we compare size to know if we need to enlarge or narrow).
    if l < 16:
        if l0 < 16:
            for i in range(16):
                adjusted_M.append([])
                for j in range(16):

                    adjusted_M[i].append(M[(int)(i * height_factor_)][(int)(j * width_factor)])
        else:
            for i in range(16):
                adjusted_M.append([])
                for j in range(16):
                    adjusted_M[i].append(M[(int)(i * height_factor_)][(int)(j // width_factor)])
    else:
        if l0 < 16:
            for i in range(16):
                adjusted_M.append([])
                for j in range(16):
                    adjusted_M[i].append(M[(int)(i // height_factor_)][(int)(j * width_factor)])

        else:
            for i in range(16):
                adjusted_M.append([])
                for j in range(16):
                    adjusted_M[i].append(M[(int)(i // height_factor_)][(int)(j // width_factor)])

    return adjusted_M


################################################################################################################
#                                               Block Cutting                                                  #
################################################################################################################


# Vertical scan of the image (matrix) in order to list the different vertical blocks
# call also the same horizontal path recursively
# until all blocks have the minimum size above the threshold
# recursivity stops when a vertical and horizontal path are negative
# we use the h or v boolean var to know if the last call was negative or not
# return the list of cut matrix
def vertical(M, h):
    # creates the histogram whose variables are the number of white pixels on each row of the matrix
    (l, l0) = (len(M), len(M[0]))

    HV = create_histogram(l0)
    for col in range(l0):
        for line in range(l):
            HV[col] += (M[line][col] // 255)

    # handles thresholding using the threshold and the full histogram
    cut_list = thresholding(HV, s, l)

    # we make a list of cut areas and if this one is empty we will stop either if
    # the horizontal call is negative either immediately if the last horizontal call was negative
    if not cut_list and not h:
        return [M]
    elif not cut_list:
        return horizontal(M, False)
    else:
        matrix_list = [M]
        matrix_list2 = []
        count = 0
        index = 0

        # proceed the matrix cutting for each column of the cut-list
        for col in cut_list:
            to_cut_matrix = matrix_list[count]
            matrix_list.pop()
            matrix_list += cut_matrix(to_cut_matrix, None, col - index)
            count += 1
            index += col

        # make a recursive horizontal call for each of the new matrix
        for matrix in matrix_list:
            matrix_list2 += horizontal(matrix, True)
        return matrix_list2


# horizontal scan of the image (matrix) in order to list the different horizontal blocks
# call also the same vertical path recursively
# until all blocks have the minimum size above the threshold
# recursivity stops when a horizontal and vertical path are negative
# we use the h or v boolean var to know if the last call was negative or not
# return the list of cut matrix
def horizontal(M, v):
    # creates the histogram whose variables are the number of white pixels on each column of the matrix
    (l, l0) = (len(M), len(M[0]))

    HH = create_histogram(l)
    for line in range(l):
        for col in range(l0):
            HH[line] += (M[line][col] // 255)

    # handles thresholding using the threshold and the full histogram
    cut_list = thresholding(HH, s, l0)

    # we make a list of cut areas and if this one is empty we will stop either if
    # the vertical call is negative either immediately if the last vertical call was negative
    if not cut_list and not v:
        return [M]
    elif not cut_list:
        return vertical(M, False)
    else:
        matrix_list = [M]
        matrix_list2 = []
        count = 0
        index = 0
        # proceed the matrix cutting for each column of the cut-list
        for line in cut_list:
            to_cut_matrix = matrix_list[count]
            matrix_list.pop()
            matrix_list += cut_matrix(to_cut_matrix, line - index, None)
            count += 1
            index += line

        # make a recursive horizontal call for each of the new matrix
        for little_matrix in matrix_list:
            matrix_list2 += vertical(little_matrix, True)
        return matrix_list2


################################################################################################################
#                                                Row Cutting                                                   #
################################################################################################################

# same as the horizontal function without the recursive call
def lines(M):
    (l, l0) = (len(M), len(M[0]))

    HL = create_histogram(l)
    for line in range(l):
        for col in range(l0):
            HL[line] += (M[line][col] // 255)

    cut_list = thresholding(HL, sl, l0)

    if not cut_list:
        return [M]
    else:
        matrix_list = [M]
        count = 0
        index = 0
        for line in cut_list:
            to_cut_matrix = matrix_list[count]
            matrix_list.pop()
            matrix_list += cut_matrix(to_cut_matrix, line - index, None)
            count += 1
            index += line

        return matrix_list


################################################################################################################
#                                            Column cutting (words&char)                                       #
################################################################################################################

# same as the vertical function without the recursive call, we add the "s" threshold because the function
# can be called to make words OR chars.
def cols(M, s):
    (l, l0) = (len(M), len(M[0]))
    HC = create_histogram(l0)
    for col in range(l0):
        for line in range(l):
            HC[col] += (M[line][col] // 255)

    cut_list = thresholding(HC, s, l)

    if not cut_list:
        return [M]
    else:
        matrix_list = [M]
        count = 0
        index = 0

        for col in cut_list:
            to_cut_matrix = matrix_list[count]
            matrix_list.pop()
            matrix_list += cut_matrix(to_cut_matrix, None, col - index)
            count += 1
            index += col

        return matrix_list


################################################################################################################
#                                       From picture to char (Main function)                                   #
################################################################################################################

# The cutting is separated in 4 parts: paragraphs,lines,words and characters.
# Each parts are put in a list in order to easy transcribe the list into a full text with the good order or letters.
# wipe_white_borders is used to optimize calls and to delete the issue if a great part of the image is white.
# At the end we resize every char_matrix into 16x16 matrix to call them in the neural network.
def from_picture_matrix_to_char_matrix(M):
    # blocks
    wipe_white_borders(M)
    M = horizontal(M, True)

    # lines
    lines_matrix = []
    for blocks in M:
        blocks = wipe_white_borders(blocks)
        lines_matrix.append(lines(blocks))

    # word
    word_matrix = []
    w = 0
    for blocks in lines_matrix:
        word_matrix.append([])
        for line in blocks:
            line = wipe_white_borders(line)
            word_matrix[w].append(cols(line, sw))
        w += 1

    # char
    char_matrix = []
    w = 0

    for blocks in word_matrix:
        char_matrix.append([])
        c = 0
        for line in blocks:
            char_matrix[w].append([])
            for word in line:
                word = wipe_white_borders(word)
                chars = cols(word, sc)
                new_chars = []
                for char in chars:
                    char = adjust_matrix_size(wipe_white_borders(char))
                    new_chars.append(char)
                char_matrix[w][c].append(new_chars)
            c += 1
        w += 1
    return char_matrix


################################################################################################################
#                                           Tests or Initialized variables                                     #
################################################################################################################


# Temporary function to convert a picture into a matrix
def from_pic_to_matrix(img):
    (h_img, l_img) = (img.size[0], img.size[1])
    M = []
    for i in range(0, h_img):
        M.append([])
        for j in range(0, l_img):
            (r, g, b) = img.getpixel((i, j))
            M[i].append((r + g + b) // 3)
    return M


def printlistofmatrix(M):
    for i in M:
        print(i)


# Thresholds for detecting paragraphs, lines ,words and chars
s = 3
sw = 2
sc = 1
sl = 1


#test with a tiny picture
"""
M = [[255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255],
     [255, 0  , 255, 255, 255, 255, 255, 255, 0  , 0  , 255, 255, 0  , 0  , 0  , 0  , 255, 255, 255, 0  , 255],
     [255, 0  , 255, 255, 255, 255, 255, 255, 0  , 0  , 255, 255, 0  , 255, 255, 0  , 255, 255, 255, 0  , 255],
     [255, 0  , 255, 255, 255, 255, 255, 255, 0  , 0  , 255, 255, 0  , 0  , 0  , 0  , 255, 255, 255, 0  , 255],
     [255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255],
     [255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255],
     [255, 0  , 255, 255, 255, 255, 255, 255, 0  , 0  , 255, 255, 0  , 255, 255, 0  , 255, 255, 255, 0  , 255],
     [255, 0  , 255, 255, 255, 255, 255, 255, 255, 0  , 255, 255, 0  , 0  , 0  , 0  , 255, 255, 255, 0  , 255],
     [255, 0  , 255, 255, 255, 255, 255, 255, 0  , 0  , 255, 255, 0  , 255, 255, 0  , 255, 255, 255, 0  , 255],
     [255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255],
     [255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255],
     [255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255],
     [255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255],
     [255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255],
     [255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255],
     [255, 0  , 255, 255, 255, 255, 255, 255, 0  , 0  , 255, 255, 255, 255, 0  , 255, 255, 255, 255, 0  , 255],
     [255, 0  , 255, 255, 255, 255, 255, 255, 255, 0  , 255, 255, 255, 0  , 0  , 0  , 255, 255, 255, 0  , 255],
     [255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255],
     [255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255]]"""

# print(from_picture_matrix_to_char_matrix(M))