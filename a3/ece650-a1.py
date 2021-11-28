from __future__ import print_function #to cover v2.7-3.x compatibility issues in print (specifying file)
"""
ECE 750 Fall 2021 Assignment 3 (program 1/3): Modified from assignment 1
Description
-----------
    The program in this file waits for user input and either modifies (add, 
    remove, modify) entries of streets (each with street name and coordinates 
    of poly-line segments of the street); or returns the edges and vertices of
    an undirected graph as required by the task. Program terminates upon
    encountering EOF (manually: Ctrl+D in Linux systems, and Ctrl+Z in Windows).

Functions
---------
eprint(arg) : Returns none. Prints arg to std error.
getUserInput(database) : Waits for user input and returns parsed input as dict. Also
                     does error handling.
getLineSegments(database) : Returns lists of line segments in each street as a dict
                            of lists from the updated database.
distance(a,b) : Returns Euclidean distance between 2-D points a and b.
collinearCheck(line_segment,point) : Returns bool value after checking if point and 
                                     line segment are collinear and point lies on the
                                     line segment.
correctCollinearEdges(edges,vertices) : Returns list of edges after correcting to
                                        ensure no 3 vertices share an edge.
removeDuplicateEdges(edges) : Returns list of edges after removing all duplicate edges
                              as graph is undirected.
getGraph(line_segments) : Returns edges and vertices of graph computed from the line 
                          segments in database according to task requirements.
main() : Directs program flow.

Created on Fri Oct 1 13:51:31 2021
Last edited on Thu Oct 14 17:23:28 2021

@author: r2balamu
"""


#Imports
import re
import sys
import copy
import math
import time

class Unbuffered(object):
   def __init__(self, stream):
       self.stream = stream
   def write(self, data):
       self.stream.write(data)
       self.stream.flush()
   def writelines(self, datas):
       self.stream.writelines(datas)
       self.stream.flush()
   def __getattr__(self, attr):
       return getattr(self.stream, attr)

sys.stdout = Unbuffered(sys.stdout)

def eprint(arg):
    """
    Prints string in 'arg' to standard error.

    Parameters
    ----------
    arg : string
        String to print.
    
    Returns
    -------
    None.

    """
    print(arg,file=sys.stderr)

#Function definitions
def getUserInput(Data):
    """
    Asks for user input and checks for errors. Returns the parsed instructions
    after each valid user entry.

    Parameters
    ----------
    Data : dict
        Dictionary of street names to ensure validity of add, mod and rm 
        commands.

    Returns
    -------
    parsed_input : dict
        Dictionary with command, coordinates and street name split up as keys.

    """
    try:
        data = copy.deepcopy(Data)
        parsed_input = {}
       # uin = sys.stdin.readline()
        uin = sys.stdin.readline()
        #Regex patterns:
        pattern1 = "(add|mod)\s\"([a-zA-Z\s]+)\"\s((\(-?\d*,-?\d*\) ?){2,})"
        pattern2 = "(rm)\s\"([a-zA-Z\s]+)\""
        pattern3 = "\"( +[a-zA-Z\s]*)\""
        pattern4 = "\)\("
        #Matching to ensure proper instruction formatting:
        matchobj1 = re.match(pattern1,uin,re.M|re.I) 
        matchobj2 = re.match(pattern2,uin,re.M|re.I)
        matchobj3 = re.search(pattern3,uin,re.M|re.I)
        matchobj4 = re.search(pattern4,uin,re.M|re.I)
        #split into command, street name, coords (parsing instruction)
        #If gg
        if "gg" in uin and not matchobj1 and not matchobj2:
            parsed_input['command'] = 'gg'
        #If rm
        elif matchobj2:
            street_name = ''.join(matchobj2.group(2).split()).lower()
            if street_name in data:
                parsed_input['command'] = matchobj2.group(1)
                parsed_input['street_name'] = street_name
            else:
                eprint("Error: 'rm' specified for street that does not exist.")
        #If mod or add
        elif matchobj1 and not matchobj3 and not matchobj4: 
            street_name = ''.join(matchobj1.group(2).split()).lower()
            if (matchobj1.group(1)=='add'):
                if (street_name not in data):
                    parsed_input['command'] = matchobj1.group(1)
                    parsed_input['street_name'] = street_name
                    parsed_input['coords'] = matchobj1.group(3)
                else:
                    eprint("Error: 'add' specified for street already added. Use 'mod' to change coordinates if needed.")
            else:
                if (street_name in data):
                    parsed_input['command'] = matchobj1.group(1)
                    parsed_input['street_name'] = street_name
                    parsed_input['coords'] = matchobj1.group(3)
                else:
                    eprint("Error: 'mod' specified for street that does not exist.")
        elif matchobj3: #Error catching
            eprint('Error: Street name not properly formatted. No leading space or empty field allowed')
        elif matchobj4: #Error catching
            eprint("Error: No space between each pair of coordinates. Correct -> (...) (...)")
        elif uin == "": #Termination
            parsed_input['command'] = 'exit'
        else:
            eprint('Error: Wrong entry format.')
    except EOFError:
        parsed_input['command'] = 'exit'
    return parsed_input

def getLineSegments(Data):
    """
    Returns dictionary containing lists of line segments of each street in 
    database 'Data' as values and street names as keys.

    Parameters
    ----------
    Data : dict
        Dictionary with street names as keys and coordinates in string type.

    Returns
    -------
    linesegs : dict
        Dictionary of streets with each street's line segments as values.

    """
    data = copy.deepcopy(Data)
    linesegs = {}
    for k,v in data.items():
        get_coords = re.findall(r'-?\d+?,[ -]?\d+?',v)
        n_coords = len(get_coords)
        coords = [[0,0]]*n_coords
        temp_dict = {}
        for i in range(n_coords):
            get_coords[i] = get_coords[i].replace(' ','')
            coords[i] = get_coords[i].split(",")
            for j in [0,1]:
                coords[i][j] = int(coords[i][j])
            if i>0:
                temp_dict[i] = [coords[i-1],coords[i]]
        linesegs[k] = temp_dict
    return linesegs

def distance(a,b):
    """
    Returns Euclidean distance between points a and b.

    Parameters
    ----------
    a : list
        Coordinates of type [x,y].
    b : list
        Coordinates of type [x,y].

    Returns
    -------
    dist : float
        Euclidean distance between the points in the argument.

    """
    dist = ((a[0]-b[0])**2+(a[1]-b[1])**2)**0.5
    return dist

def collinearCheck(ls1,point):
    """
    Checks collinearity of point with line segment 'ls1' and returns True or
    False accordingly.

    Parameters
    ----------
    ls1 : list
        Pair of coordinates of form [[x1,y1],[x2,y2]].
    point : list
        Coordinates of form [x,y].

    Returns
    -------
    flag : bool
        True if collinear, False if non-collinear.

    """
    flag = False
    p0_x,p0_y = ls1[0][0],ls1[0][1]
    p1_x,p1_y = ls1[1][0],ls1[1][1]
    p2_x,p2_y = point[0],point[1]
    if (math.floor((p2_y-p0_y)*(p1_x-p0_x))==math.floor((p1_y-p0_y)*(p2_x-p0_x)) and
        math.floor((p1_y-p2_y)*(p1_x-p0_x))==math.floor((p1_y-p0_y)*(p1_x-p2_x))):
        checkdist = distance(ls1[0],ls1[1])
        if (distance(ls1[0],point)<checkdist and 
            distance(ls1[1],point)<checkdist):
                flag = True
    return flag

def correctCollinearEdges(org_edges,verts):
    """
    Returns list of edges after correcting the naive original list to ensure
    all edges connect adjacent vertices only.

    Parameters
    ----------
    org_edges : list
        Original list of edges.
    verts : dict
        Dictionary of vertices with ids as keys and coordinates as values.

    Returns
    -------
    edges : list
        List of [id_A,id_B] pairs where A and B are different vertices 
        sharing an edge.

    """
    edges = copy.deepcopy(org_edges)
    all_set = False
    while all_set != True:
        new_edges = list()
        check_count = 0
        for i in range(len(edges)):
            ls_ids = edges[i]
            ls = [verts[ls_ids[0]],verts[ls_ids[1]]]
            flag = False
            for j in verts:
                if j != ls_ids[0] and j != ls_ids[1]:
                    if collinearCheck(ls,verts[j])==True:
                        new_edges.append([ls_ids[0],j])
                        new_edges.append([ls_ids[1],j])
                        flag = True
            if flag == True:
                check_count += 1
            else:
                new_edges.append(edges[i])
        if check_count == 0:
            all_set = True
        edges = copy.deepcopy(new_edges)
    return edges

def removeDuplicateEdges(org_edges):
    """
    Returns list of edges after removing all duplicates in it (graph is 
    undirected).

    Parameters
    ----------
    org_edges : list
        Original list of all edges, each as pairs of vertex indices.

    Returns
    -------
    new_edges : list
        Resultant edge list after removal of duplicates.

    """
    edges = copy.deepcopy(org_edges)
    eds_to_remove = list()
    for coord in edges:
        if coord[0] == coord[1]:
            edges.remove(coord)
    for i in range(len(edges)-1):
        for j in range(i+1,len(edges)):
            if (edges[i]==edges[j] or (edges[i][0]==edges[j][1] and
                                       edges[i][1]==edges[j][0])):
                eds_to_remove.append(j)
    new_edges = [edges[x] for x in range(len(edges)) if x not in eds_to_remove]
    return new_edges

def getGraph(linesegs):
    """
    Returns edges and vertices of undirected graph according to task 
    requirements from entered coordinates after every command that results 
    in the data changing (add,mod,rm).

    Parameters
    ----------
    linesegs : dict
        Dictionary with street names as keys and lists of line
        segments in each street as respective values.

    Returns
    -------
    verts : dict 
        Dictionary of vertices, keys are index and values are coordinates
        of vertices.
    new_edges : list
        List of vertex pairs with edges between the listed pairs.

    """
    lineSegs = copy.deepcopy(linesegs) #copying since dicts are mutable
    # First, getting all vertices and edges naively:
    verts = dict()
    edges = list()
    keys2 = list()
    keys1 = list(lineSegs.keys())
    count = 0
    for v in lineSegs.values():
        keys2.append(list(v.keys()))
    for i in range(len(keys1)-1):
        lsi = linesegs[keys1[i]]
        for j in range(i+1,len(keys1)):
            lsj = lineSegs[keys1[j]]
            for x in range(len(lsi)):
                for y in range(len(lsj)):
                    int_x = 0
                    int_y = 0
                    ls_1 = lsi[keys2[i][x]]
                    ls_2 = lsj[keys2[j][y]]
                    p0_x,p0_y = ls_1[0][0],ls_1[0][1]
                    p1_x,p1_y = ls_1[1][0],ls_1[1][1]
                    p2_x,p2_y = ls_2[0][0],ls_2[0][1]
                    p3_x,p3_y = ls_2[1][0],ls_2[1][1]
                    s1_x = p1_x-p0_x
                    s2_x = p3_x-p2_x
                    s1_y = p1_y-p0_y
                    s2_y = p3_y-p2_y
                    det = float(-s2_x*s1_y+s1_x*s2_y)
                    if det != 0: #If not collinear or parallel
                        s = float(-s1_y*(p0_x-p2_x)+s1_x*(p0_y-p2_y))/det
                        t = float(s2_x*(p0_y-p2_y)-s2_y*(p0_x-p2_x))/det
                        if (s>=0 and s<=1 and t>=0 and t<=1): #check for 
                        #intersection and do:
                            int_x = float(p0_x+(t*s1_x))
                            int_y = float(p0_y+(t*s1_y))
                            #Add intersection and 4 ends of line segments as 
                            #vertices:
                            verts[count] = [int_x,int_y]
                            verts[count+1] = [p0_x,p0_y]
                            verts[count+2] = [p1_x,p1_y]
                            verts[count+3] = [p2_x,p2_y]
                            verts[count+4] = [p3_x,p3_y]
                            #Add edges naively first:
                            edges.append([count,count+1])
                            edges.append([count,count+2])
                            edges.append([count,count+3])
                            edges.append([count,count+4])
                            
                            count = count+5
    
    #Replace all duplicate vertices with a single id
    uniq_vs = list()
    vs_to_remove = list()
    for i in verts:
        if verts[i] not in uniq_vs:
            uniq_vs.append(verts[i])
            for j in verts:
                if i != j and verts[j]==verts[i]:
                    vs_to_remove.append(j)
                    for x in range(len(edges)):
                        for y in [0,1]:
                            if edges[x][y] == j:
                                edges[x][y] = i
        else:
            vs_to_remove.append(i)
    vs_to_remove = set(vs_to_remove) #To ensure no duplicate vertices
    for i in vs_to_remove:
        del verts[i]
    
    #Correcting the naively computed edges
    new_edges = removeDuplicateEdges(edges)
    new_edges = correctCollinearEdges(new_edges, verts)
    new_edges = removeDuplicateEdges(new_edges)
    
    return verts,new_edges

def main():
    """
    Main function that waits for user input, modifies database of entries as
    instructed by user and displays resultant graph on request. Program 
    terminates when parser encounters EOF or user presses special button 
    combination for EOF (Ctrl+D in Linux, Ctrl+Z in Windows).

    Returns
    -------
    None.

    """
    database = dict()
    vertices = dict()
    edges = list()
    while True:
        user_input = getUserInput(database) #dict
        if user_input=={}:
            pass
        else:
            #From user input, get command
            command = user_input['command']
            if command != "exit":
                #If add,mod,rm- compute/re-compute edges, vertices
                if command != "gg":
                    if command != "rm":
                        database[user_input['street_name']] = user_input['coords']
                    else:
                        del database[user_input['street_name']]
                    
                    lsegs = getLineSegments(database) #dict
                    vertices,edges = getGraph(lsegs) #dict,list
                
                else:
                    n_verts = len(vertices)
                    new_edges = copy.deepcopy(edges)
                    old_vertex_ids = list(vertices.keys())
                    sys.stdout.write("V {!r}\n".format(n_verts))
                    for i,old_ids in enumerate(edges):
                        for j,ids in enumerate(old_vertex_ids):
                            if old_ids[0]==ids:
                                new_edges[i][0]= j+1
                            if old_ids[1]==ids:
                                new_edges[i][1]= j+1
                    sys.stdout.write("E {"+",".join("<{!r},{!r}>".format(item[0],item[1])
                        for ids,item in enumerate(new_edges)) + "}\n")
                    time.sleep(2)
                    sys.stdout.flush()
                    time.sleep(1)
            else:
                break
    sys.exit(0)

if __name__=="__main__":
    main()
